#include <linuxnet.h>
#include <sys/socket.h>

extern int socketcall(int callno,long* args);

#ifdef __i386__
int __libc_getpeername(int a);
int __libc_getpeername(int a) {
  return socketcall(SYS_GETPEERNAME, (long*)&a);
#else
int __libc_getpeername(int a, void * b, int *c);
int __libc_getpeername(int a, void * b, int *c) {
  unsigned long args[] = { a, (long) b, (long) c };
  return socketcall(SYS_GETPEERNAME, args);
#endif
}

int getpeername(int a, struct sockaddr* b, socklen_t *c) __attribute__((weak,alias("__libc_getpeername")));
