#include <linux/net.h>

extern int socketcall(int callno,long* args);

int __libc_connect(int a, void * b, int c) {
#ifdef __i386__
  return socketcall(SYS_CONNECT, (long*)&a);
#else
  unsigned long args[] = { a, (long) b, c };
  return socketcall(SYS_CONNECT, args);
#endif
}

int connect(int a, void * b, int c) __attribute__((weak,alias("__libc_connect")));
