#include <linuxnet.h>

extern int socketcall(int callno,long* args);

#ifdef __i386__
int socket(int a) {
  return socketcall(SYS_SOCKET, (long*)&a);
#else
int socket(int a, int b, int c) {
  unsigned long args[] = { a, b, c };
  return socketcall(SYS_SOCKET, args);
#endif
}
