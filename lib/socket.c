#include <linux/net.h>

extern int socketcall(int callno,long* args);

int socket(int a, int b, int c) {
#ifdef __i386__
  return socketcall(SYS_SOCKET, (long*)&a);
#else
  unsigned long args[] = { a, b, c };
  return socketcall(SYS_SOCKET, args);
#endif
}
