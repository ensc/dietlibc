#include <linux/net.h>

extern int socketcall(int callno,long* args);

int socketpair(int a, int type, int protocol, int sv[2]) {
#ifdef __i386__
  return socketcall(SYS_SOCKETPAIR, (long*)&a);
#else
  unsigned long args[] = { a, type, protocol, sv };
  return socketcall(SYS_SOCKETPAIR, args);
#endif
}
