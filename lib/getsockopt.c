#include <linux/net.h>

extern int socketcall(int callno,long* args);

#ifdef __i386__
int getsockopt(int a) {
  return socketcall(SYS_GETSOCKOPT, (long*)&a);
#else
int getsockopt(int a, int b, int c, void *d, int e) {
  unsigned long args[] = { a, b, c, (long)d, e };
  return socketcall(SYS_GETSOCKOPT, args);
#endif
}

