#include <linux/net.h>

extern int socketcall(int callno,long* args);

int __libc_accept(int a, void * addr, void * addr2) {
#ifdef __i386__
  return socketcall(SYS_ACCEPT, (long*)&a);
#else
  unsigned long args[] = { a, (long) addr, (long) addr2 };
  return socketcall(SYS_ACCEPT, args);
#endif
}

int accept(int a, void * addr, void * addr2) __attribute__((weak,alias("__libc_accept")));
