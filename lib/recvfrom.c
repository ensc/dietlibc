#include <sys/types.h>
#include <linuxnet.h>

extern int socketcall(int callno,long* args);

#ifdef __i386__
int __libc_recvfrom(int a);
int __libc_recvfrom(int a) {
  return socketcall(SYS_RECVFROM, (long*)&a);
#else
int __libc_recvfrom(int a, const void * b, size_t c, int flags, void *to, void *tolen);
int __libc_recvfrom(int a, const void * b, size_t c, int flags, void *to, void *tolen) {
  unsigned long args[] = { a, (long) b, c, flags, (long) to, (long) tolen };
  return socketcall(SYS_RECVFROM, args);
#endif
}

int recvfrom(int a, const void * b, size_t c, int flags, void *to, void *tolen)
 __attribute__ ((weak,alias("__libc_recvfrom"))) ;
