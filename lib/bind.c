#include <linux/net.h>

extern int socketcall(int callno,long* args);

int bind(int a, void * b, int c) {
#ifdef __i386__
  return socketcall(SYS_BIND, (long*)&a);
#else
  unsigned long args[] = { a, (long) b, c };
  return socketcall(SYS_BIND, args);
#endif
}
