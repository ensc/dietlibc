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

extern int h_errno;

static char dnspacket[]="\xfe\xfe\001\000\000\001\000\000\000\000\000\000";

extern void __dns_make_fd();
extern int __dns_fd;

extern int __dns_servers;
extern struct sockaddr __dns_server_ips[];

extern void __dns_readstartfiles();

extern int __dns_decodename(unsigned char *packet,int offset,unsigned char *dest,int maxlen);

/* Oh boy, this interface sucks so badly, there are no words for it.
 * Not one, not two, but _three_ error signalling methods!  (*h_errnop
 * nonzero?  return value nonzero?  *RESULT zero?)  The glibc goons
 * really outdid themselves with this one. */
int __dns_gethostbyx_r(const char* name, struct hostent* result,
			char *buf, size_t buflen,
			struct hostent **RESULT, int *h_errnop, int lookfor) {
  int names,ips;
  unsigned char *cur;
  unsigned char *max;
  unsigned char packet[512];
  __dns_make_fd();

  if (lookfor==1) {
    result->h_aliases=(char**)(buf+8*4);
    result->h_addrtype=AF_INET;
    result->h_length=4;
    result->h_addr_list=(char**)buf;
  } else {
    result->h_aliases=(char**)(buf+8*16);
    result->h_addrtype=AF_INET6;
    result->h_length=16;
    result->h_addr_list=(char**)buf;
  }
  result->h_aliases[0]=0;

  cur=buf+16*sizeof(char*);
  max=buf+buflen;
  names=ips=0;

  memmove(packet,dnspacket,12);
  *(unsigned short*)packet=rand();
  {
    unsigned char* x;
    const char* y,* tmp;
    x=packet+12; y=name;
    while (*y) {
      while (*y=='.') ++y;
      for (tmp=y; *tmp && *tmp!='.'; ++tmp) ;
      *x=tmp-y;
      if (!(tmp-y)) break;
      ++x;
      if (x>=packet+510-(tmp-y)) { *h_errnop=ERANGE; return 1; }
      memmove(x,y,tmp-y);
      x+=tmp-y;
      if (!*tmp) {
	*x=0;
	break;
      }
      y=tmp;
    }
    *++x= 0; *++x= lookfor;	/* A */
    *++x= 0; *++x= 1;	/* IN */
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
	  /*int len=*/ read(__dns_fd,inpkg,1500);
	  /* header, question, answer, authority, additional */
	  if (inpkg[0]!=packet[0] || inpkg[1]!=packet[1]) continue;	/* wrong ID */
	  if ((inpkg[2]&0xf9) != 0x81) continue;	/* not answer */
	  if ((inpkg[3]&0x0f) != 0) break;		/* error */
	  tmp=inpkg+12;
	  {
	    char name[257];
	    unsigned short q=((unsigned short)inpkg[4]<<8)+inpkg[5];
	    while (q>0) {
	      while (*tmp) tmp+=*tmp+1;
	      tmp+=5;
	      --q;
	    }
	    q=((unsigned short)inpkg[6]<<8)+inpkg[7];
	    if (q<1) break;
	    while (q>0) {
	      int decofs=__dns_decodename(inpkg,tmp-(char*)inpkg,name,256);
	      if (decofs<0) break;
	      tmp=inpkg+decofs;
	      --q;
	      if (tmp[0]!=0 || tmp[1]!=lookfor ||	/* TYPE != A */
	          tmp[2]!=0 || tmp[3]!=1) {		/* CLASS != IN */
		if (tmp[1]==5) {	/* CNAME */
		  tmp+=10;
		  decofs=__dns_decodename(inpkg,tmp-(char*)inpkg,name,256);
		  if (decofs<0) break;
		  tmp=inpkg+decofs;
		} else
		  break;
		continue;
	      }
	      tmp+=10;	/* skip type, class, TTL and length */
	      {
		int slen;
		if (lookfor==1 || lookfor==28) /* A or AAAA*/ {
		  slen=strlen(name);
		  if (cur+slen+8+(lookfor==28?12:0)>=max) { *h_errnop=NO_RECOVERY; return 1; }
		} else if (lookfor==12) /* PTR */ {
		  decofs=__dns_decodename(inpkg,tmp-(char*)inpkg,name,256);
		  if (decofs<0) break;
		  tmp=inpkg+decofs;
		  slen=strlen(name);
		} else
		  slen=strlen(name);
		strcpy(cur,name);
		if (names==0)
		  result->h_name=cur;
		else
		  result->h_aliases[names-1]=cur;
		result->h_aliases[names]=0;
		++names;
/*		cur+=slen+1; */
		cur+=(slen|3)+1;
		result->h_addr_list[ips++] = cur;
		if (lookfor==1) /* A */ {
		  *(int*)cur=*(int*)tmp;
		  cur+=4;
		  result->h_addr_list[ips]=0;
		} else if (lookfor==28) /* AAAA */ {
		  {
		    int i;
		    for (i=0; i<16; ++i) cur[i]=tmp[i];
		  }
		  cur+=16;
		  result->h_addr_list[ips]=0;
		}
	      }
/*	      puts(name); */
	    }
	  }
/*	  printf("%d answers\n",((unsigned short)inpkg[6]<<8)+inpkg[7]);
	  printf("ok\n");*/
	  *h_errnop=0;
	  *RESULT=result;
	  return 0;
	}
/*kaputt:*/
      }
    }
  }
  return 1;
}
