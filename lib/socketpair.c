#include <linuxnet.h>

extern int socketcall(int callno,long* args);

#ifdef __i386__
int __libc_socketpair(int a);
int __libc_socketpair(int a) {
  return socketcall(SYS_SOCKETPAIR, (long*)&a);
#else
int __libc_socketpair(int a, int type, int protocol, int sv[2]);
int __libc_socketpair(int a, int type, int protocol, int sv[2]) {
  unsigned long args[] = { a, type, protocol, sv };
  return socketcall(SYS_SOCKETPAIR, args);
#endif
}

int socketpair(int d, int type, int protocol, int sv[2])
  __attribute__((weak,alias("__libc_socketpair")));
