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
#include <fcntl.h>
#include "dietfeatures.h"

extern void __dns_make_fd(void);
extern int __dns_fd;
#ifdef WANT_IPV6_DNS
extern void __dns_make_fd6(void);
extern int __dns_fd6;
#endif

extern void __dns_readstartfiles(void);

int res_query(const char *dname, int class, int type, unsigned char *answer, int anslen) {
  unsigned char packet[512];
  int size;
#ifndef WANT_IPV6_DNS
  __dns_make_fd();
#endif

  __dns_readstartfiles();
  if ((size=res_mkquery(QUERY,dname,class,type,0,0,0,packet,512))<0) { h_errno=NO_RECOVERY; return -1; }
  {
    {
      int i;	/* current server */
      int j;	/* timeout count down */
      struct timeval last,now;
#ifdef WANT_IPV6_PLUGPLAY_DNS
      static int pnpfd=-1;
      static struct sockaddr_in6 pnpsa;
      struct pollfd duh[2];

      if (pnpfd<0) {
	pnpfd=socket(PF_INET6,SOCK_DGRAM,IPPROTO_UDP);
	if (pnpfd>=0) fcntl(pnpfd,F_SETFD,FD_CLOEXEC);
      }
      memset(&pnpsa,0,sizeof(pnpsa));
      pnpsa.sin6_family=AF_INET6;
      if (pnpfd!=-1) bind(pnpfd,(struct sockaddr*)&pnpsa,sizeof(pnpsa));
      pnpsa.sin6_port=htons(53);
      memmove(&pnpsa.sin6_addr,"\xff\x02\x00\x00\x00\x00\x00\x00\x00\x00dnspnp",16);

      duh[1].events=POLLIN;
      duh[1].fd=pnpfd;

      if (strchr(dname,'.')) {
	duh[1].fd=-1;
	duh[1].revents=0;
      }
#else
      struct pollfd duh[1];
#endif
      i=0;
      duh[0].events=POLLIN;
      last.tv_sec=0;
#ifdef WANT_IPV6_PLUGPLAY_DNS
      if (duh[1].fd!=-1)
	sendto(pnpfd,packet,size,0,(struct sockaddr*)(&pnpsa),sizeof(struct sockaddr_in6));
      /* if it doesn't work, we don't care */
#endif
      for (j=120; j>0; --j) {
	gettimeofday(&now,0);
	if (now.tv_sec-last.tv_sec>10) {
#ifdef WANT_IPV6_DNS
	  int tmpfd;
	  struct sockaddr* s=(struct sockaddr*)&(_res.nsaddr_list[i]);
	  if (s->sa_family==AF_INET6) {
	    __dns_make_fd6();
	    tmpfd=__dns_fd6;
	  } else {
	    __dns_make_fd();
	    tmpfd=__dns_fd;
	  }
#ifdef WANT_IPV6_PLUGPLAY_DNS
	  if (duh[0].fd!=-1) {
#endif
	  duh[0].fd=tmpfd;
	  if (sendto(tmpfd,packet,size,0,s,sizeof(struct sockaddr_in6))==0)
	    gettimeofday(&last,0);
#ifdef WANT_IPV6_PLUGPLAY_DNS
	  }
#endif
#else
	  duh[0].fd=__dns_fd;
	  if (sendto(__dns_fd,packet,size,0,(struct sockaddr*)&(_res.nsaddr_list[i]),sizeof(struct sockaddr))==0)
	    gettimeofday(&last,0);
#endif
	  last=now;
	}
	if (++i >= _res.nscount) i=0;
#ifdef WANT_IPV6_PLUGPLAY_DNS
	if (duh[0].fd==-1 && duh[1].fd==-1) goto nxdomain;
	duh[0].revents=0;
	if (poll(duh[0].fd==-1?duh+1:duh,duh[0].fd==-1?1:2,1000) > 0) {
#else
	if (poll(duh,1,1000) == 1) {
#endif
	  /* read and parse answer */
	  unsigned char inpkg[1500];
#ifdef WANT_IPV6_PLUGPLAY_DNS
	  int len;
	  len=read(duh[0].revents&POLLIN?duh[0].fd:duh[1].fd,inpkg,1500);
#else
	  int len=read(duh[0].fd,inpkg,1500);
#endif
	  /* header, question, answer, authority, additional */
	  if (inpkg[0]!=packet[0] || inpkg[1]!=packet[1]) continue;	/* wrong ID */
	  if ((inpkg[2]&0xf9) != (_res.options&RES_RECURSE?0x81:0x80)) continue;	/* not answer */
	  if ((inpkg[3]&0x0f) != 0) {
#ifdef WANT_IPV6_PLUGPLAY_DNS
/* if the normal DNS server says NXDOMAIN, still give the multicast method some time */
	    if (duh[0].revents&POLLIN) {
	      duh[0].fd=-1;
	      if (duh[1].fd!=-1) {
		if (j>10) j=10;
		continue;
	      }
	    } else
	      continue;
/* ignore NXDOMAIN from the multicast socket */
nxdomain:
#endif
	    h_errno=HOST_NOT_FOUND;
	    return -1;
	  }		/* error */
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

