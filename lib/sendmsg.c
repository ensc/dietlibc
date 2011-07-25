#include "syscalls.h"
#ifdef __NR_socketcall

#include <sys/socket.h>
#include <linuxnet.h>

extern int socketcall(int callno,long* args);

ssize_t __libc_sendmsg(int a, const struct msghdr* msg, int flags);
ssize_t __libc_sendmsg(int a, const struct msghdr* msg, int flags) {
  long args[] = { a, (long) msg, flags };
  return socketcall(SYS_SENDMSG, args);
}

ssize_t sendmsg(int a, const struct msghdr *msg, int flags)
 __attribute__ ((weak,alias("__libc_sendmsg"))) ;

#endif
