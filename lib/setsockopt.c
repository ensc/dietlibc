#include <linuxnet.h>
#include <sys/socket.h>

extern int socketcall(int callno,long* args);

#ifdef __i386__
int __libc_setsockopt(int a);
int __libc_setsockopt(int a) {
  return socketcall(SYS_SETSOCKOPT, (long*)&a);
#else
int __libc_setsockopt(int a, int b, int c, void *d, void *e);
int __libc_setsockopt(int a, int b, int c, void *d, void *e) {
  unsigned long args[] = { a, b, c, (long)d, (long) e };
  return socketcall(SYS_SETSOCKOPT, args);
#endif
}

int setsockopt(int s, int level, int optname, const void* optval, socklen_t optlen) __attribute__((weak,alias("__libc_setsockopt")));
