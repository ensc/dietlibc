#include <signal.h>

extern int __sigsuspend(int a,int b,unsigned long c);

int __libc_sigsuspend(const sigset_t *mask) {
  return __sigsuspend(0,0,mask->sig[0]);
}

int sigsuspend(const sigset_t *mask) __attribute__((weak,alias("__libc_sigsuspend")));
