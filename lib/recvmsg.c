#include <sys/socket.h>
#include <linuxnet.h>

extern int socketcall(int callno,long* args);

#ifdef __i386__
int __libc_recvmsg(int a);
int __libc_recvmsg(int a) {
  return socketcall(SYS_RECVMSG, (long*)&a);
#else
int __libc_recvmsg(int a, struct msghdr* msg, int flags);
int __libc_recvmsg(int a, struct msghdr* msg, int flags) {
  unsigned long args[] = { a, (long) msg, flags };
  return socketcall(SYS_RECVMSG, args);
#endif
}

int recvmsg(int a, struct msghdr *msg, int flags)
 __attribute__ ((weak,alias("__libc_recvmsg"))) ;
