#include <linuxnet.h>

extern int socketcall(int callno,long* args);

#ifdef __i386__
int bind(int a) {
  return socketcall(SYS_BIND, (long*)&a);
#else
int bind(int a, void * b, int c) {
  unsigned long args[] = { a, (long) b, c };
  return socketcall(SYS_BIND, args);
#endif
}
