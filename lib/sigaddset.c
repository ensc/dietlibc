#include <signal.h>

int __sigaddset(sigset_t *set, int sig) {
  *set |= (1UL << (sig-1));
  return 0;
}

int sigaddset (sigset_t *env, int signo) __attribute__((weak,alias("__sigaddset")));
