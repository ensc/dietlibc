#include <linuxnet.h>
#include <sys/socket.h>

extern int socketcall(int callno,long* args);

#ifdef __i386__
int __libc_getsockname(int a);
int __libc_getsockname(int a) {
  return socketcall(SYS_GETSOCKNAME, (long*)&a);
#else
int __libc_getsockname(int a, void * b, int c);
int __libc_getsockname(int a, void * b, int c) {
  unsigned long args[] = { a, (long) b, c };
  return socketcall(SYS_GETSOCKNAME, args);
#endif
}

int getsockname(int a, struct sockaddr* b, socklen_t* c) __attribute__((weak,alias("__libc_getsockname")));
