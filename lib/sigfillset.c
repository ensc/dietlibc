#include <signal.h>

int __sigfillset (sigset_t *set) {
  memset(set,0xff,sizeof(*set));
  return 0;
}

int sigfillset(sigset_t *env) __attribute__((weak,alias("__sigfillset")));
