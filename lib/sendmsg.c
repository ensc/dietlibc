#include <sys/socket.h>
#include <linuxnet.h>

extern int socketcall(int callno,long* args);

#ifdef __i386__
int __libc_sendmsg(int a);
int __libc_sendmsg(int a) {
  return socketcall(SYS_SENDMSG, (long*)&a);
#else
int __libc_sendmsg(int a, const struct msghdr* msg, int flags);
int __libc_sendmsg(int a, const struct msghdr* msg, int flags) {
  unsigned long args[] = { a, (long) msg, flags };
  return socketcall(SYS_SENDMSG, args);
#endif
}

int sendmsg(int a, const struct msghdr *msg, int flags)
 __attribute__ ((weak,alias("__libc_sendmsg"))) ;
