#include <linux/net.h>

extern int socketcall(int callno,long* args);

int __libc_recvfrom(int a, const void * b, size_t c, int flags, void *to, void *tolen) {
#ifdef __i386__
  return socketcall(SYS_RECVFROM, (long*)&a);
#else
  unsigned long args[] = { a, (long) b, c, flags, (long) to, (long) tolen };
  return socketcall(SYS_RECVFROM, args);
#endif
}

int recvfrom(int a, const void * b, size_t c, int flags, void *to, void *tolen)
 __attribute__ ((weak,alias("__libc_recvfrom"))) ;
