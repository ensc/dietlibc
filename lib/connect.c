#include <linuxnet.h>

extern int socketcall(int callno,long* args);

#ifdef __i386__
int __libc_connect(int a);
int __libc_connect(int a) {
  return socketcall(SYS_CONNECT, (long*)&a);
#else
int __libc_connect(int a, void * b, int c) {
int __libc_connect(int a, void * b, int c);
  unsigned long args[] = { a, (long) b, c };
  return socketcall(SYS_CONNECT, args);
#endif
}

int connect(int a, void * b, int c) __attribute__((weak,alias("__libc_connect")));
