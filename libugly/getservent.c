#include <unistd.h>
#include <fcntl.h>
#include <netdb.h>
#include <sys/mman.h>
#include <errno.h>
#include <netinet/in.h>
#include <string.h>
#include <ctype.h>
#include "dietfeatures.h"

static int servicesfd=-1;
static char* servicesmap;
static unsigned int serviceslen;

static char* aliases[10];

static char *cur;

/* nameserver	42/tcp		name		# IEN 116 */
struct servent *getservent(void) {
  static struct servent se;
  char *last;
  int aliasidx;
  if (servicesfd<0) {
    servicesfd=open(_PATH_SERVICES,O_RDONLY);
    if (servicesfd<0) return 0;
    serviceslen=lseek(servicesfd,0,SEEK_END);
    servicesmap=mmap(0,serviceslen,PROT_READ|PROT_WRITE,MAP_PRIVATE,servicesfd,0);
    if ((long)servicesmap==(-1)) goto error;
    cur=servicesmap;
  }
  last=servicesmap+serviceslen;
again:
  se.s_name=0;
  se.s_aliases=aliases; aliases[0]=0;
  se.s_port=0;
  se.s_proto=0;
  if (cur>=last) return 0;
  if (*cur=='#' || *cur=='\n') goto parseerror;
  /* first, the primary name */
  if (!isalpha(*cur)) goto parseerror;
  se.s_name=cur;
  while (cur<last && isalnum(*cur)) cur++;
  if (cur>=last) return 0;
  if (*cur=='\n') goto parseerror;
  *cur=0; cur++;
  /* second, the port */
  while (cur<last && isblank(*cur)) cur++;
  while (cur<last && isdigit(*cur)) {
    se.s_port=se.s_port*10+*cur-'0';
    cur++;
  }
  se.s_port=htons(se.s_port);
  if (cur>=last) return 0;
  /* third, "/tcp" or "/udp" */
  if (*cur!='/') goto parseerror;
  cur++;
  se.s_proto=cur;
  while (cur<last && isalpha(*cur)) ++cur;
  if (cur>=last) return 0;
  if (*cur=='\n') { *cur++=0; return &se; }
  *cur=0; cur++;
  /* now the aliases */
  for (aliasidx=0;aliasidx<10;++aliasidx) {
    while (cur<last && isblank(*cur)) ++cur;
    aliases[aliasidx]=cur;
    while (cur<last && !isspace(*cur)) ++cur;
    if (*cur=='\n') { *cur++=0; ++aliasidx; break; }
    if (cur>=last || !isblank(*cur)) break;
    *cur++=0;
  }
  aliases[aliasidx]=0;
  return &se;
parseerror:
  while (cur<last && *cur!='\n') cur++;
  cur++;
  goto again;
error:
  if (servicesmap!=(char*)-1) munmap(servicesmap,serviceslen);
  if (servicesfd!=-1) close(servicesfd);
  servicesmap=(char*)-1;
  servicesfd=-1;
  errno=ENOMEM;
  return 0;
}

struct servent *getservbyport(int port, const char *proto) {
  struct servent *s;
  endservent();
  for (s=getservent(); s; s=getservent()) {
    if (port==s->s_port && !strcmp(proto,s->s_proto))
      return s;
  }
  return 0;
}

void endservent(void) {
  if (servicesmap!=(char*)-1) munmap(servicesmap,serviceslen);
  if (servicesfd!=-1) close(servicesfd);
  servicesmap=(char*)-1;
  servicesfd=-1;
}

void setservent(int stayopen) {
  endservent();
}

struct servent *getservbyname(const char *name, const char *proto) {
  struct servent *s;
  setservent(0);
  for (s=getservent(); s; s=getservent()) {
    char **tmp;
#if 0
    write(1,"found ",6);
    write(1,s->s_name,strlen(s->s_name));
    write(1,"/",1);
    write(1,s->s_proto,strlen(s->s_proto));
    write(1,"\n",1);
    if (!strcmp(s->s_name,"pop3")) {
      write(2,"ssh!\n",5);
    }
#endif
    if (strcmp(proto,s->s_proto)) continue;
    if (!strcmp(name,s->s_name))
      return s;
    tmp=s->s_aliases;
    while (*tmp)
      if (!strcmp(name,*tmp++)) return s;
  }
  return 0;
}

