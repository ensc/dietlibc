#include "syscalls.h"
#ifdef __NR_socketcall

#include <sys/types.h>
#include <linuxnet.h>

extern int socketcall(int callno,long* args);

ssize_t __libc_send(int a, const void * b, size_t c, int flags);
ssize_t __libc_send(int a, const void * b, size_t c, int flags) {
  long args[] = { a, (long) b, c, flags };
  return socketcall(SYS_SEND, args);
}

ssize_t send(int a, const void * b, size_t c, int flags)
  __attribute__ ((weak, alias("__libc_send")));

#endif
