#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>

/* XXX TODO FIXME */

int getaddrinfo(const char *node, const char *service, const struct addrinfo *hints, struct addrinfo **res) {
  struct addrinfo **tmp;
  int family;
  tmp=res; *res=0;
  for (family=PF_INET6; ; family=PF_INET) {
    if (!hints || hints->ai_family==family || hints->ai_family==AF_UNSPEC) {	/* IPv6 addresses are OK */
      struct hostent h;
      struct hostent *H;
      int herrno;
      char buf[4096];
      if (!gethostbyname2_r(node,family,&h,buf,4096,&H,&herrno)) {
	struct ai_v6 {
	  struct addrinfo ai;
	  union {
	    struct sockaddr_in6 ip6;
	    struct sockaddr_in ip4;
	  } ip;
	  char name[1];
	} *foo;
	unsigned short port;
	int len=sizeof(struct ai_v6)+strlen(h.h_name);
	if (!(foo=malloc(len))) goto error;
	foo->ai.ai_next=0;
	foo->ai.ai_socktype=SOCK_STREAM;
	foo->ai.ai_protocol=IPPROTO_TCP;
	foo->ai.ai_addrlen=family==PF_INET6?16:4;
	foo->ai.ai_addr=(struct sockaddr*)&foo->ip;
	foo->ai.ai_canonname=foo->name;
	foo->ip.ip6.sin6_family=foo->ai.ai_family=family;
	if (family==PF_INET6) {
	  memmove(&foo->ip.ip6.sin6_addr,h.h_addr_list[0],16);
	  foo->ip.ip6.sin6_flowinfo=foo->ip.ip6.sin6_scope_id=0;
	} else {
	  memmove(&foo->ip.ip4.sin_addr,h.h_addr_list[0],4);
	}
	memmove(foo->name,h.h_name,strlen(h.h_name)+1);
	if (!hints || hints->ai_socktype!=SOCK_DGRAM) {	/* TCP is OK */
	  char *x;
	  port=strtol(service,&x,0);
	  if (*x) {	/* service is not numeric :-( */
	    struct servent* se;
	    if ((se=getservbyname(service,"tcp"))) {	/* found a service. */
	      port=se->s_port;
  blah1:
	      if (family==PF_INET6)
		foo->ip.ip6.sin6_port=port;
	      else
		foo->ip.ip4.sin_port=port;
	      if (!*tmp) *tmp=&(foo->ai); else (*tmp)->ai_next=&(foo->ai);
	      if (!(foo=malloc(len))) goto error;
	      memmove(foo,*tmp,len);
	      tmp=&(*tmp)->ai_next;
	      foo->ai.ai_addr=(struct sockaddr*)&foo->ip;
	      foo->ai.ai_canonname=foo->name;
	    }
	  } else goto blah1;
	}
	foo->ai.ai_socktype=SOCK_DGRAM;
	foo->ai.ai_protocol=IPPROTO_UDP;
	if (!hints || hints->ai_socktype!=SOCK_STREAM) {	/* UDP is OK */
	  char *x;
	  port=strtol(service,&x,0);
	  if (*x) {	/* service is not numeric :-( */
	    struct servent* se;
	    if ((se=getservbyname(service,"udp"))) {	/* found a service. */
	      port=se->s_port;
  blah2:
	      if (family==PF_INET6)
		foo->ip.ip6.sin6_port=port;
	      else
		foo->ip.ip4.sin_port=port;
	      if (!*tmp) *tmp=&(foo->ai); else (*tmp)->ai_next=&(foo->ai);
	      if (!(foo=malloc(len))) goto error;
	      memmove(foo,*tmp,len);
	      tmp=&(*tmp)->ai_next;
	      foo->ai.ai_addr=(struct sockaddr*)&foo->ip;
	      foo->ai.ai_canonname=foo->name;
	    }
	  } else goto blah2;
	}
	free(foo);
      }
    }
    if (family==PF_INET) break;
  }
  return 0;
error:
  freeaddrinfo(*res);
  return -1;
}
