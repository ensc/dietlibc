#include <linux/net.h>

extern int socketcall(int callno,long* args);

int setsockopt(int a, int b, int c, void *d, void *e) {
#ifdef __i386__
  return socketcall(SYS_SETSOCKOPT, (long*)&a);
#else
  unsigned long args[] = { a, b, c, (long)d, (long) e };
  return socketcall(SYS_SETSOCKOPT, args);
#endif
}

