#include <linux/net.h>

extern int socketcall(int callno,long* args);

int __libc_shutdown(int s, int how) {
#ifdef __i386__
  return socketcall(SYS_SHUTDOWN, (long*)&s);
#else
  unsigned long args[] = { s, (long) how, 0 };
  return socketcall(SYS_SHUTDOWN, args);
#endif
}

int shutdown(int s, int how) __attribute__((weak,alias("__libc_shutdown")));
