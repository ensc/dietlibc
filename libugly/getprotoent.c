#include <unistd.h>
#include <fcntl.h>
#include <netdb.h>
#include <sys/mman.h>
#include <errno.h>
#include <netinet/in.h>
#include <string.h>
#include <ctype.h>
#include "dietfeatures.h"

static int protofd=-1;
static char* protomap;
static unsigned int protolen;

static char* aliases[10];

static char *cur;

/* ip	0	IP		# internet protocol, pseudo protocol number */
struct protoent *getprotoent(void) {
  static struct protoent pe;
  char *last;
  int aliasidx;
  if (protofd<0) {
    protofd=open(_PATH_PROTOCOLS,O_RDONLY);
    if (protofd<0) return 0;
    protolen=lseek(protofd,0,SEEK_END);
    protomap=mmap(0,protolen,PROT_READ|PROT_WRITE,MAP_PRIVATE,protofd,0);
    if ((long)protomap==(-1)) goto error;
    cur=protomap;
  }
  last=protomap+protolen;
again:
  pe.p_name=0;
  pe.p_aliases=aliases; aliases[0]=0;
  pe.p_proto=0;
  if (cur>=last) return 0;
  if (*cur=='#' || *cur=='\n') goto parseerror;
  /* first, the primary name */
  if (!isalpha(*cur)) goto parseerror;
  pe.p_name=cur;
  pe.p_aliases=aliases;
  while (cur<last && isalnum(*cur)) cur++;
  if (cur>=last) return 0;
  if (*cur=='\n') goto parseerror;
  *cur=0; cur++;
  /* second, the protocol number */
  while (cur<last && isblank(*cur)) cur++;
  while (cur<last && isdigit(*cur)) {
    pe.p_proto=pe.p_proto*10+*cur-'0';
    cur++;
  }
/*  pe.p_proto=htons(pe.s_proto); */
  if (cur>=last) return 0;
  if (*cur=='\n') { *cur++=0; return &pe; }
  *cur=0; cur++;
  /* now the aliases */
  for (aliasidx=0;aliasidx<10;++aliasidx) {
    while (cur<last && isblank(*cur)) ++cur;
    aliases[aliasidx]=cur;
    while (cur<last && isalpha(*cur)) ++cur;
    if (*cur=='\n') { *cur++=0; ++aliasidx; break; }
    if (cur>=last || !isblank(*cur)) break;
    *cur++=0;
  }
  aliases[aliasidx]=0;
  return &pe;
parseerror:
  while (cur<last && *cur!='\n') cur++;
  cur++;
  goto again;
error:
  if (protomap!=(char*)-1) munmap(protomap,protolen);
  if (protofd!=-1) close(protofd);
  protomap=(char*)-1;
  protofd=-1;
  errno=ENOMEM;
  return 0;
}

void setprotoent(int stayopen) {
  cur=protomap;
}

struct protoent *getprotobyname(const char *name) {
  struct protoent *s;
  endprotoent();
  for (s=getprotoent(); s; s=getprotoent()) {
    char **tmp;
#if 0
    write(1,"found ",6);
    write(1,s->p_name,strlen(s->p_name));
    write(1,"/",1);
    write(1,s->p_proto,strlen(s->p_proto));
    write(1,"\n",1);
    if (!strcmp(name,"auth")) {
      tmp=s->p_aliases;
      write(1,"  aka ",5);
      while (*tmp) {
	write(1,*tmp,strlen(*tmp));
	write(1,", ",2);
	++tmp;
      }
      write(1,"\n",1);
    }
#endif
    if (!strcmp(name,s->p_name))
      return s;
    tmp=s->p_aliases;
    while (*tmp)
      if (!strcmp(name,*tmp++)) return s;
  }
  return 0;
}

struct protoent *getprotobynumber(int proto) {
  struct protoent *s;
  endprotoent();
  for (s=getprotoent(); s; s=getprotoent()) {
    if (proto==s->p_proto)
      return s;
  }
  return 0;
}

void endprotoent(void) {
  if (protomap!=(char*)-1) munmap(protomap,protolen);
  if (protofd!=-1) close(protofd);
  protomap=(char*)-1;
  protofd=-1;
}

