#include <linuxnet.h>
#include <sys/socket.h>

extern int socketcall(int callno,long* args);

#ifdef __i386__
int __libc_shutdown(int s);
int __libc_shutdown(int s) {
  return socketcall(SYS_SHUTDOWN, (long*)&s);
#else
int __libc_shutdown(int s, int how);
int __libc_shutdown(int s, int how) {
  unsigned long args[] = { s, (long) how, 0 };
  return socketcall(SYS_SHUTDOWN, args);
#endif
}

int shutdown(int s, int how) __attribute__((weak,alias("__libc_shutdown")));
