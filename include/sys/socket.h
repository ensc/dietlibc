#ifndef _SYS_SOCKET_H
#define _SYS_SOCKET_H

#include <sys/cdefs.h>
#include <sys/types.h>
#include <linux/socket.h>

struct sockaddr_storage {
  sa_family_t  ss_family;
  uint32_t  __ss_align;
  char __ss_padding[(128  - (2 * sizeof (uint32_t ))) ];
};

#ifndef SOCK_DGRAM
/* the Linux kernel headers suck really badly on non-x86 */
#define SOCK_STREAM	1		/* stream (connection) socket	*/
#define SOCK_DGRAM	2		/* datagram (conn.less) socket	*/
#define SOCK_RAW	3		/* raw socket			*/
#define SOCK_RDM	4		/* reliably-delivered message	*/
#define SOCK_SEQPACKET	5		/* sequential packet socket	*/
#define SOCK_PACKET	10		/* linux specific way of	*/
#endif

int socket(int domain, int type, int protocol) __THROW;
int accept(int s, struct sockaddr *addr, socklen_t *addrlen) __THROW;
int connect(int sockfd, const struct sockaddr *serv_addr, socklen_t addrlen) __THROW;
int bind(int sockfd, struct sockaddr *my_addr, socklen_t addrlen) __THROW;
int recv(int s, void *buf, size_t len, int flags) __THROW;
int recvfrom(int s, void *buf, size_t len, int flags, struct sockaddr *from, socklen_t *fromlen) __THROW;
int recvmsg(int s, struct msghdr *msg, int flags) __THROW;
int send(int s, const void *msg, size_t len, int flags) __THROW;
int sendto(int s, const void *msg, size_t len, int flags, const struct sockaddr *to, socklen_t tolen) __THROW;
int sendmsg(int s, const struct msghdr *msg, int flags) __THROW;

int getpeername(int s, struct sockaddr *name, socklen_t *namelen) __THROW;
int getsockname(int  s , struct sockaddr * name , socklen_t * namelen) __THROW;

int getsockopt(int s, int level, int optname, void *optval, socklen_t *optlen) __THROW;
int setsockopt(int s, int level, int optname, const void *optval, socklen_t optlen) __THROW;

int listen(int s, int backlog) __THROW;

#define SHUT_RD 0
#define SHUT_WR 1
#define SHUT_RDWR 2
int shutdown(int s, int how) __THROW;

/* currently not supported: */
#define NI_NOFQDN 1

#define NI_NUMERICHOST 2
#define NI_NAMEREQD 4
#define NI_NUMERICSERV 8
#define NI_DGRAM 16

struct addrinfo {
  int     ai_flags;
  int     ai_family;
  int     ai_socktype;
  int     ai_protocol;
  size_t  ai_addrlen;
  struct sockaddr *ai_addr;
  char   *ai_canonname;
  struct addrinfo *ai_next;
};

int getnameinfo(const struct sockaddr *sa, socklen_t salen, char *host,
		size_t hostlen, char *serv, size_t servlen, int flags) __THROW;
int getaddrinfo(const char *node, const char *service, const struct
		addrinfo *hints, struct addrinfo **res) __THROW;
void freeaddrinfo(struct addrinfo *res) __THROW;
const char *gai_strerror(int errcode) __THROW;

#define EAI_FAMILY -1
#define EAI_SOCKTYPE -2
#define EAI_BADFLAGS -3
#define EAI_NONAME -4
#define EAI_SERVICE -5
#define EAI_ADDRFAMILY -6
#define EAI_NODATA -7
#define EAI_MEMORY -8
#define EAI_FAIL -9
#define EAI_AGAIN -10
#define EAI_SYSTEM -11

#define AI_NUMERICHOST 1
#define AI_CANONNAME 2
#define AI_PASSIVE 4

#endif
