#include <sys/types.h>
#include <linuxnet.h>

extern int socketcall(int callno,long* args);

#ifdef __i386__
int __libc_send(int a);
int __libc_send(int a) {
  return socketcall(SYS_SEND, (long*)&a);
#else
int __libc_send(int a, const void * b, size_t c, int flags);
int __libc_send(int a, const void * b, size_t c, int flags) {
  unsigned long args[] = { a, (long) b, c, flags };
  return socketcall(SYS_SEND, args);
#endif
}

int send(int a, const void * b, size_t c, int flags)
  __attribute__ ((weak, alias("__libc_send")));
