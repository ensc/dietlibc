#include <resolv.h>
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
#include <arpa/nameser.h>
#include "dietfeatures.h"

extern int h_errno;

extern void __dns_make_fd(void);
extern int __dns_fd;

extern void __dns_readstartfiles(void);

extern int __dns_decodename(unsigned char *packet,unsigned int offset,unsigned char *dest,unsigned int maxlen);

int res_query(const char *dname, int class, int type, unsigned char *answer, int anslen) {
  unsigned char packet[512];
  int size;
  __dns_make_fd();

  if ((size=res_mkquery(QUERY,dname,C_IN,type,0,0,0,packet,512))<0) return 1;
  {
    {
      int i;	/* current server */
      int j;	/* timeout count down */
      struct pollfd duh;
      i=0; j=30;
      __dns_readstartfiles();
      duh.fd=__dns_fd;
      duh.events=POLLIN;
      for (j=30; j>0; --j) {
	sendto(__dns_fd,packet,size,0,(struct sockaddr*)&(_res.nsaddr_list[i]),sizeof(struct sockaddr));
	if (++i > _res.nscount) i=0;
	if (poll(&duh,1,1) == 1) {
	  /* read and parse answer */
	  unsigned char inpkg[1500];
	  int len=read(__dns_fd,inpkg,1500);
	  /* header, question, answer, authority, additional */
	  if (inpkg[0]!=packet[0] || inpkg[1]!=packet[1]) continue;	/* wrong ID */
	  if ((inpkg[2]&0xf9) != 0x81) continue;	/* not answer */
	  if ((inpkg[3]&0x0f) != 0) break;		/* error */
	  if (len>anslen)
	    return -1;
	  memmove(answer,inpkg,len);
	  return len;
	}
/*kaputt:*/
      }
    }
  }
  return 1;
}

