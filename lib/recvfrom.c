#include "syscalls.h"
#ifdef __NR_socketcall

#include <sys/types.h>
#include <linuxnet.h>

extern int socketcall(int callno,long* args);

ssize_t __libc_recvfrom(int a, const void * b, size_t c, int flags, void *to, void *tolen);
ssize_t __libc_recvfrom(int a, const void * b, size_t c, int flags, void *to, void *tolen) {
  long args[] = { a, (long) b, c, flags, (long) to, (long) tolen };
  return socketcall(SYS_RECVFROM, args);
}

ssize_t recvfrom(int a, const void * b, size_t c, int flags, void *to, void *tolen)
 __attribute__ ((weak,alias("__libc_recvfrom"))) ;

#endif
