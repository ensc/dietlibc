#define __KERNEL__
#include <asm/signal.h>

int __sigfillset (sigset_t *set) {
  int i;
  for (i=0; i<_NSIG_WORDS; i++)
    set->sig[i]=~0;
  return 0;
}

int sigfillset(sigset_t *env) __attribute__((weak,alias("__sigfillset")));
