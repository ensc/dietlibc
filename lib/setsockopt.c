#include <linux/net.h>

extern int socketcall(int callno,long* args);

#ifdef __i386__
int setsockopt(int a) {
  return socketcall(SYS_SETSOCKOPT, (long*)&a);
#else
int setsockopt(int a, int b, int c, void *d, void *e) {
  unsigned long args[] = { a, b, c, (long)d, (long) e };
  return socketcall(SYS_SETSOCKOPT, args);
#endif
}

