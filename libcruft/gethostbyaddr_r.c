#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/poll.h>
#include <unistd.h>
#include <errno.h>

#include <stdio.h>

extern int __dns_gethostbyx_r(const char* name, struct hostent* result,
			char *buf, size_t buflen,
			struct hostent **RESULT, int *h_errnop, int lookfor);

static int i2a(char* dest,unsigned int x) {
  register unsigned int tmp=x;
  register int len=0;
  if (x>=100) { *dest++=tmp/100+'0'; tmp=tmp%100; ++len; }
  if (x>=10) { *dest++=tmp/10+'0'; tmp=tmp%10; ++len; }
  *dest++=tmp+'0';
  return len+1;
}

/* Oh boy, this interface sucks so badly, there are no words for it.
 * Not one, not two, but _three_ error signalling methods!  (*h_errnop
 * nonzero?  return value nonzero?  *RESULT zero?)  The glibc goons
 * really outdid themselves with this one. */
int gethostbyaddr_r(const char* addr, size_t length, int format,
		    struct hostent* result, char *buf, size_t buflen,
		    struct hostent **RESULT, int *h_errnop) {
  if (format==AF_INET) {
    char tmpbuf[50];
    char *tmp;
    int res;
    tmp=tmpbuf+i2a(tmpbuf,(unsigned char)addr[3]); *tmp++='.';
    tmp+=i2a(tmp,(unsigned char)addr[2]); *tmp++='.';
    tmp+=i2a(tmp,(unsigned char)addr[1]); *tmp++='.';
    tmp+=i2a(tmp,(unsigned char)addr[0]); strcpy(tmp,".in-addr.arpa");
    res= __dns_gethostbyx_r(tmpbuf,result,buf+4,buflen-4,RESULT,h_errnop,12);	/* 12 == ns_t_ptr */
    if (res==0) {
      result->h_addr_list[0]=buf;
      result->h_addr_list[1]=buf;
      *(int*)buf=*(int*)addr;
    }
    return res;
  }
  return 1;
}
