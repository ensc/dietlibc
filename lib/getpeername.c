#include <linuxnet.h>

extern int socketcall(int callno,long* args);

#ifdef __i386__
int getpeername(int a) {
  return socketcall(SYS_GETPEERNAME, (long*)&a);
#else
int getpeername(int a, void * b, int c) {
  unsigned long args[] = { a, (long) b, c };
  return socketcall(SYS_GETPEERNAME, args);
#endif
}

