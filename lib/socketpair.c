#include <linux/net.h>

extern int socketcall(int callno,long* args);

#ifdef __i386__
int socketpair(int a) {
  return socketcall(SYS_SOCKETPAIR, (long*)&a);
#else
int socketpair(int a, int type, int protocol, int sv[2]) {
  unsigned long args[] = { a, type, protocol, sv };
  return socketcall(SYS_SOCKETPAIR, args);
#endif
}
