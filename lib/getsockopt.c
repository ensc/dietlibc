#include <linuxnet.h>
#include <sys/socket.h>

extern int socketcall(int callno,long* args);

#ifdef __i386__
int __libc_getsockopt(int a);
int __libc_getsockopt(int a) {
  return socketcall(SYS_GETSOCKOPT, (long*)&a);
#else
int __libc_getsockopt(int a, int b, int c, void *d, int e);
int __libc_getsockopt(int a, int b, int c, void *d, int e) {
  unsigned long args[] = { a, b, c, (long)d, e };
  return socketcall(SYS_GETSOCKOPT, args);
#endif
}

int getsockopt(int s, int level, int optname, void * optval, socklen_t *optlen) __attribute__((weak,alias("__libc_getsockopt")));
