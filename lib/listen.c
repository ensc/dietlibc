#include <linux/net.h>

extern int socketcall(int callno,long* args);

int listen(int a, int b) {
#ifdef __i386__
  return socketcall(SYS_LISTEN, (long*)&a);
#else
  unsigned long args[] = { a, b, 0 };
  return socketcall(SYS_LISTEN, args);
#endif
}

