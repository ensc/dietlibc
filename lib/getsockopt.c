#include <linux/net.h>

extern int socketcall(int callno,long* args);

int getsockopt(int a, int b, int c, void *d, int e) {
#ifdef __i386__
  return socketcall(SYS_GETSOCKOPT, (long*)&a);
#else
  unsigned long args[] = { a, b, c, (long)d, e };
  return socketcall(SYS_GETSOCKOPT, args);
#endif
}

