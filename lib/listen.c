#include <linuxnet.h>
#include <sys/socket.h>

extern int socketcall(int callno,long* args);

#ifdef __i386__
int __libc_listen(int a);
int __libc_listen(int a) {
  return socketcall(SYS_LISTEN, (long*)&a);
#else
int __libc_listen(int a, int b);
int __libc_listen(int a, int b) {
  unsigned long args[] = { a, b, 0 };
  return socketcall(SYS_LISTEN, args);
#endif
}

int listen(int s, int backlog) __attribute__((weak,alias("__libc_listen")));
