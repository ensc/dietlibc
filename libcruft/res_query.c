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

static char dnspacket[]="\xfe\xfe\001\000\000\001\000\000\000\000\000\000";

extern void __dns_make_fd(void);
extern int __dns_fd;

extern int __dns_servers;
extern struct sockaddr __dns_server_ips[];

extern void __dns_readstartfiles(void);

extern int __dns_decodename(unsigned char *packet,unsigned int offset,unsigned char *dest,unsigned int maxlen);

int res_query(const char *dname, int class, int type, unsigned char *answer, int anslen) {
  unsigned char packet[512];
  __dns_make_fd();

  memmove(packet,dnspacket,12);
  *(unsigned short*)packet=rand();
  {
    unsigned char* x;
    const char* y,* tmp;
    x=packet+12; y=dname;
    while (*y) {
      while (*y=='.') ++y;
      for (tmp=y; *tmp && *tmp!='.'; ++tmp) ;
      *x=tmp-y;
      if (!(tmp-y)) break;
      ++x;
      if (x>=packet+510-(tmp-y)) { return -1; }
      memmove(x,y,tmp-y);
      x+=tmp-y;
      if (!*tmp) {
	*x=0;
	break;
      }
      y=tmp;
    }
    *++x= 0; *++x= type;	/* A */
    *++x= 0; *++x= class;	/* IN */
    ++x;
    {
      int i;	/* current server */
      int j;	/* timeout count down */
      struct pollfd duh;
      i=0; j=30;
      __dns_readstartfiles();
      duh.fd=__dns_fd;
      duh.events=POLLIN;
      for (j=30; j>0; --j) {
	sendto(__dns_fd,packet,x-packet,0,(struct sockaddr*)&(__dns_server_ips[i]),sizeof(struct sockaddr));
	if (++i > __dns_servers) i=0;
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

