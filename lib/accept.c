#include <linuxnet.h>

extern int socketcall(int callno,long* args);

#ifdef __i386__
int __libc_accept(int a);

int __libc_accept(int a) {
  return socketcall(SYS_ACCEPT, (long*)&a);
#else
int __libc_accept(int a, void * addr, void * addr2);

int __libc_accept(int a, void * addr, void * addr2) {
  unsigned long args[] = { a, (long) addr, (long) addr2 };
  return socketcall(SYS_ACCEPT, args);
#endif
}

int accept(int a, void * addr, void * addr2) __attribute__((weak,alias("__libc_accept")));
