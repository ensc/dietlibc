#ifndef _SYS_SOCKET_H
#define _SYS_SOCKET_H

#include <sys/cdefs.h>
#include <sys/types.h>
#include <linux/socket.h>

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

#endif
