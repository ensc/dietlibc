#include <linux/net.h>

extern int socketcall(int callno,long* args);

int __libc_sendmsg(int a, const struct msghdr* msg, int flags) {
#ifdef __i386__
  return socketcall(SYS_SENDMSG, (long*)&a);
#else
  unsigned long args[] = { a, (long) msg, flags };
  return socketcall(SYS_SENDMSG, args);
#endif
}

int sendmsg(int a, const struct msghdr *msg, int flags)
 __attribute__ ((weak,alias("__libc_sendmsg"))) ;
