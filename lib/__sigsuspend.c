#include <signal.h>

extern int __sigsuspend(int a,int b,long c);

int __libc_sigsuspend(const sigset_t *mask) {
  return __sigsuspend(0,0,*mask);
}

int sigsuspend(const sigset_t *mask) __attribute__((weak,alias("__libc_sigsuspend")));
