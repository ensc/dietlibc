#include <linux/net.h>

extern int socketcall(int callno,long* args);

int __libc_recvmsg(int a, struct msghdr* msg, int flags) {
#ifdef __i386__
  return socketcall(SYS_RECVMSG, (long*)&a);
#else
  unsigned long args[] = { a, (long) msg, flags };
  return socketcall(SYS_RECVMSG, args);
#endif
}

int recvmsg(int a, struct msghdr *msg, int flags)
 __attribute__ ((weak,alias("__libc_recvmsg"))) ;
