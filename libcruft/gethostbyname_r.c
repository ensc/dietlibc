#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "dietfeatures.h"
#include <errno.h>
#include "dietdns.h"

/* Oh boy, this interface sucks so badly, there are no words for it.
 * Not one, not two, but _three_ error signalling methods!  (*h_errnop
 * nonzero?  return value nonzero?  *RESULT zero?)  The glibc goons
 * really outdid themselves with this one. */
int gethostbyname_r(const char* name, struct hostent* result,
				char *buf, size_t buflen,
				struct hostent **RESULT, int *h_errnop) {
  size_t L=strlen(name);
  result->h_name=buf;
  if (buflen<L) { *h_errnop=ERANGE; return 1; }
  strcpy(buf,name);
#ifdef WANT_ETC_HOSTS
  {
    struct hostent* r;
    while ((r=gethostent_r(buf,buflen))) {
      int i;
      if (r->h_addrtype==AF_INET && !strcmp(r->h_name,name)) {	/* found it! */
found:
	memmove(result,r,sizeof(struct hostent));
	*RESULT=result;
	*h_errnop=0;
	endhostent();
	return 0;
      }
      for (i=0; i<16; ++i) {
	if (r->h_aliases[i]) {
	  if (!strcmp(r->h_aliases[i],name)) goto found;
	} else break;
      }
    }
    endhostent();
  }
#endif
  return __dns_gethostbyx_r(name,result,buf+L,buflen-L,RESULT,h_errnop,1);
}
