#include <linux/net.h>

extern int socketcall(int callno,long* args);

#ifdef __i386__
int listen(int a) {
  return socketcall(SYS_LISTEN, (long*)&a);
#else
int listen(int a, int b) {
  unsigned long args[] = { a, b, 0 };
  return socketcall(SYS_LISTEN, args);
#endif
}

