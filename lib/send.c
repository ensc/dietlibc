#include <linux/net.h>

extern int socketcall(int callno,long* args);

int __libc_send(int a, const void * b, size_t c, int flags) {
#ifdef __i386__
  return socketcall(SYS_SEND, (long*)&a);
#else
  unsigned long args[] = { a, (long) b, c, flags };
  return socketcall(SYS_SEND, args);
#endif
}

int send(int a, const void * b, size_t c, int flags)
  __attribute__ ((weak, alias("__libc_send")));
