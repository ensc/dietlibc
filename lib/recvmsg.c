#include "syscalls.h"
#ifdef __NR_socketcall

#include <sys/socket.h>
#include <linuxnet.h>

extern int socketcall(int callno,long* args);

ssize_t __libc_recvmsg(int a, struct msghdr* msg, int flags);
ssize_t __libc_recvmsg(int a, struct msghdr* msg, int flags) {
  long args[] = { a, (long) msg, flags };
  return socketcall(SYS_RECVMSG, args);
}

ssize_t recvmsg(int a, struct msghdr *msg, int flags)
 __attribute__ ((weak,alias("__libc_recvmsg"))) ;

#endif
