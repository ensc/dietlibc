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

extern void __dns_make_fd(void);
extern int __dns_fd;

extern void __dns_readstartfiles(void);

extern int __dns_decodename(unsigned char *packet,unsigned int offset,unsigned char *dest,unsigned int maxlen);

int res_query(const char *dname, int class, int type, unsigned char *answer, int anslen) {
  unsigned char packet[512];
  int size;
  __dns_make_fd();

  __dns_readstartfiles();
  if ((size=res_mkquery(QUERY,dname,class,type,0,0,0,packet,512))<0) { h_errno=NO_RECOVERY; return -1; }
  {
    {
      int i;	/* current server */
      int j;	/* timeout count down */
      struct pollfd duh;
      struct timeval last,now;
      i=0;
      duh.fd=__dns_fd;
      duh.events=POLLIN;
      for (j=10; j>0; --j) {
	gettimeofday(&now,0);
	if (now.tv_sec-last.tv_sec>10) {
	  sendto(__dns_fd,packet,size,0,(struct sockaddr*)&(_res.nsaddr_list[i]),sizeof(struct sockaddr));
	  gettimeofday(&last,0);
	}
	if (++i >= _res.nscount) i=0;
	if (poll(&duh,1,1000) == 1) {
	  /* read and parse answer */
	  unsigned char inpkg[1500];
	  int len=read(__dns_fd,inpkg,1500);
	  /* header, question, answer, authority, additional */
	  if (inpkg[0]!=packet[0] || inpkg[1]!=packet[1]) continue;	/* wrong ID */
	  if ((inpkg[2]&0xf9) != (_res.options&RES_RECURSE?0x81:0x80)) continue;	/* not answer */
	  if ((inpkg[3]&0x0f) != 0) { h_errno=HOST_NOT_FOUND; return -1; }		/* error */
	  if (len>anslen) {
	    h_errno=NO_RECOVERY;
	    return -1;
	  }
	  memmove(answer,inpkg,len);
	  return len;
	}
/*kaputt:*/
      }
    }
  }
  h_errno=NO_DATA;
  return -1;
}

