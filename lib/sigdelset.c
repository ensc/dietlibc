#define __KERNEL__
#define sigdelset foobar
#include <asm/signal.h>
#undef sigdelset

int __sigdelset(sigset_t *set, int signo) {
  unsigned long sig = signo - 1;
  if (_NSIG_WORDS == 1)
    set->sig[0] &= ~(1UL << sig);
  else
    set->sig[sig / _NSIG_BPW] &= ~(1UL << (sig % _NSIG_BPW));
  return 0;
}

int sigdelset (sigset_t *env, int signo) __attribute__((weak,alias("__sigdelset")));
