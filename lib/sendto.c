#include "syscalls.h"
#ifdef __NR_socketcall

#include <sys/socket.h>
#include <linuxnet.h>

extern int socketcall(int callno,long* args);

ssize_t __libc_sendto(int a, const void * b, size_t c, int flags, void *to, int tolen);
ssize_t __libc_sendto(int a, const void * b, size_t c, int flags, void *to, int tolen) {
  long args[] = { a, (long) b, c, flags, (long) to, tolen };
  return socketcall(SYS_SENDTO, args);
}

ssize_t sendto(int a, const void * b, size_t c, int flags, const struct sockaddr* to, socklen_t tolen)
  __attribute__ ((weak, alias("__libc_sendto")));

#endif
