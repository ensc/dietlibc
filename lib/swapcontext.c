#include <ucontext.h>
#include <stdio.h>

int swapcontext(ucontext_t* o,const ucontext_t* n) {
  volatile int once=0;
  if (getcontext(o)==-1) return -1;
  if (!once) {
    ++once;
    return setcontext(n);
  }
  return 0;
}
