#include <linuxnet.h>

extern int socketcall(int callno,long* args);

#ifdef __i386__
int getsockname(int a) {
  return socketcall(SYS_GETSOCKNAME, (long*)&a);
#else
int getsockname(int a, void * b, int c) {
  unsigned long args[] = { a, (long) b, c };
  return socketcall(SYS_GETSOCKNAME, args);
#endif
}

