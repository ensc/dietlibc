#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "dietfeatures.h"

int __dns_fd=-1;

void __dns_make_fd() {
  int tmp;
  struct sockaddr_in si;
  if (__dns_fd>0) return;
  tmp=socket(PF_INET,SOCK_DGRAM,IPPROTO_UDP);
  if (tmp<0) return;
  si.sin_family=AF_INET;
  si.sin_port=0;
  si.sin_addr.s_addr=INADDR_ANY;
  if (bind(tmp,(struct sockaddr*)&si,sizeof(si))) return;
  __dns_fd=tmp;
}

int __dns_servers=0;
struct sockaddr __dns_server_ips[8];

#ifdef WANT_FULL_RESOLV_CONF
int __dns_search;
char *__dns_domains[8];
#endif

void __dns_readstartfiles() {
  int fd;
  char *buf=alloca(4096);
  int len;
  if (__dns_servers>0) return;
  {
    struct sockaddr_in to;
    char *cacheip=getenv("DNSCACHEIP");
#ifdef WANT_FULL_RESOLV_CONF
    __dns_search=0;
#endif
    if (cacheip) {
      to.sin_port=htons(53);
      to.sin_family=AF_INET;
      if (inet_aton(cacheip,&to.sin_addr)) {
	memmove(__dns_server_ips,&to,sizeof(struct sockaddr));
	++__dns_servers;
      }
    }
  }
  if ((fd=open("/etc/resolv.conf",O_RDONLY))<0) return;
  len=read(fd,buf,4096);
  close(fd);
  {
    char *last=buf+len;
    for (; buf<last;) {
      if (!strncmp(buf,"nameserver",10)) {
	buf+=10;
	while (buf<last && *buf!='\n') {
	  while (buf<last && isblank(*buf)) ++buf;
	  {
	    char *tmp=buf;
	    struct sockaddr_in i;
	    while (buf<last && !isspace(*buf)) ++buf;
	    if (buf>=last) break;
	    *buf=0;
	    if (inet_aton(tmp,&i.sin_addr)) {
	      i.sin_family=AF_INET;
	      i.sin_port=htons(53);
	      memmove(&__dns_server_ips[__dns_servers],&i,sizeof(struct sockaddr));
	      if (__dns_servers<8) ++__dns_servers;
	    }
	  }
	}
      }
#ifdef WANT_FULL_RESOLV_CONF
      if (!strncmp(buf,"search",6) || !strncmp(buf,"domain",6)) {
	buf+=6;
	while (buf<last && isblank(*buf)) {
	  while (buf<last && isblank(*buf)) ++buf;
	  __dns_domains[__dns_search]=buf;
	  while (buf<last && (*buf=='.' || *buf=='-' || isalnum(*buf))) ++buf;
	  if (buf<last) *buf++=0;
	  if ((__dns_domains[__dns_search]=strdup(__dns_domains[__dns_search])))
	    ++__dns_search;
	}
	continue;
      }
#endif
      while (buf<last && *buf!='\n') ++buf;
      while (buf<last && *buf=='\n') ++buf;
    }
  }
}

/* return length of decoded data or -1 */
int __dns_decodename(unsigned char *packet,int offset,unsigned char *dest,int maxlen) {
  unsigned char *tmp;
  unsigned char *max=dest+maxlen;
  unsigned char *after=packet+offset;
  int ok=0;
  for (tmp=after; maxlen>0&&*tmp; ) {
    if ((*tmp>>6)==3) {		/* goofy DNS decompression */
      unsigned int ofs=((unsigned int)(*tmp&0x3f)<<8)|*(tmp+1);
      if (ofs>=offset) return -1;	/* RFC1035: "pointer to a _prior_ occurrance" */
      if (after<tmp+2) after=tmp+2;
      tmp=packet+ofs;
      ok=0;
    } else {
      unsigned int duh;
      if (dest+*tmp+1>max) return -1;
      for (duh=*tmp; duh>0; --duh)
	*dest++=*++tmp;
      *dest++='.'; ok=1;
      ++tmp;
      if (tmp>after) { after=tmp; if (!*tmp) ++after; }
    }
  }
  if (ok) --dest;
  *dest=0;
  return after-packet;
}
