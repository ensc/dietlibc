#include <linuxnet.h>

extern int socketcall(int callno,long* args);

#ifdef __i386__
int __libc_bind(int a);
int __libc_bind(int a) {
  return socketcall(SYS_BIND, (long*)&a);
#else
int __libc_bind(int a, void * b, int c);
int __libc_bind(int a, void * b, int c) {
  unsigned long args[] = { a, (long) b, c };
  return socketcall(SYS_BIND, args);
#endif
}

int bind(int a, void * b, int c) __attribute__((weak,alias("__libc_bind")));
