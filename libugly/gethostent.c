#include <unistd.h>
#include <fcntl.h>
#include <netdb.h>
#include <sys/mman.h>
#include <errno.h>
#include <netinet/in.h>
#include <string.h>
#include <ctype.h>
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "dietfeatures.h"

static char* hostmap=0;
static unsigned int hostlen;

static char *cur;

/* ::1	localhost6	alias1 alias2		# internet protocol, pseudo protocol number */
struct hostent* gethostent_r(char* buf, int len) {
  char *dest;
  struct hostent* pe=(struct hostent*)buf;
  char* last;
  char* max=buf+len;
  int aliasidx;
  if (!hostmap) {
    int hostfd=open(_PATH_HOSTS,O_RDONLY);
    if (hostfd<0) return 0;
    hostlen=lseek(hostfd,0,SEEK_END);
    hostmap=mmap(0,hostlen,PROT_READ|PROT_WRITE,MAP_PRIVATE,hostfd,0);
    if ((long)hostmap==(-1)) { close(hostfd); hostmap=0; goto error; }
    close(hostfd); hostfd=-1;
    cur=hostmap;
  }
  last=hostmap+hostlen;
again:
  if ((size_t)len<sizeof(struct hostent)+11*sizeof(char*)) goto nospace;
  dest=buf+sizeof(struct hostent);
  pe->h_name=0;
  pe->h_aliases=(char**)dest; pe->h_aliases[0]=0; dest+=10*sizeof(char*);
  pe->h_addr_list=(char**)dest; dest+=2*sizeof(char**);
  if (cur>=last) return 0;
  if (*cur=='#' || *cur=='\n') goto parseerror;
  /* first, the ip number */
  pe->h_name=cur;
  while (cur<last && !isspace(*cur)) cur++;
  if (cur>=last) return 0;
  if (*cur=='\n') goto parseerror;
  {
    char save=*cur;
    *cur=0;
    pe->h_addr_list[0]=dest;
    pe->h_addr_list[1]=0;
    if (max-dest<16) goto nospace;
    if (inet_pton(AF_INET6,pe->h_name,dest)>0) {
      pe->h_addrtype=AF_INET6;
      pe->h_length=16;
      dest+=16;
    } else if (inet_pton(AF_INET,pe->h_name,dest)>0) {
      pe->h_addrtype=AF_INET;
      pe->h_length=4;
      dest+=4;
    } else {
      *cur=save;
      goto parseerror;
    }
    *cur=save;
  }
  ++cur;
  /* now the aliases */
  for (aliasidx=0;aliasidx<9;++aliasidx) {
    while (cur<last && isblank(*cur)) ++cur;
    pe->h_aliases[aliasidx]=cur;
    while (cur<last && !isspace(*cur)) ++cur;
    {
      char *from=pe->h_aliases[aliasidx];
      int len=cur-from;
      if (max-dest<len+2) goto nospace;
      pe->h_aliases[aliasidx]=dest;
      memmove(dest,from,(size_t)(cur-from));
      dest+=len;
      *dest=0; ++dest;
    }
    if (*cur=='\n') { ++cur; ++aliasidx; break; }
    if (cur>=last || !isblank(*cur)) break;
    cur++;
  }
  pe->h_aliases[aliasidx]=0;
  pe->h_name=pe->h_aliases[0];
  pe->h_aliases++;
  return pe;
parseerror:
  while (cur<last && *cur!='\n') cur++;
  cur++;
  goto again;
nospace:
  errno=ERANGE;
  goto __error;
error:
  errno=ENOMEM;
__error:
  if (hostmap!=(char*)-1) munmap(hostmap,hostlen);
  hostmap=(char*)-1;
  return 0;
}

void sethostent(int stayopen) {
  (void)stayopen;
  cur=hostmap;
}

#if 0
struct protoent *getprotobyname(const char *name) {
  struct protoent *s;
  setprotoent(0);
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
  setprotoent(0);
  for (s=getprotoent(); s; s=getprotoent()) {
    if (proto==s->p_proto)
      return s;
  }
  return 0;
}
#endif

void endhostent(void) {
  if (hostmap!=(char*)-1) munmap(hostmap,hostlen);
  hostmap=0;
}

