#include <linuxnet.h>

extern int socketcall(int callno,long* args);

#ifdef __i386__
int __libc_socket(int a);
int __libc_socket(int a) {
  return socketcall(SYS_SOCKET, (long*)&a);
#else
int __libc_socket(int a, int b, int c);
int __libc_socket(int a, int b, int c) {
  unsigned long args[] = { a, b, c };
  return socketcall(SYS_SOCKET, args);
#endif
}

int socket(int a,int b,int c) __attribute__((weak,alias("__libc_socket")));
