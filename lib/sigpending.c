#include <signal.h>

int sigpending(sigset_t *set) {
  return __rt_sigpending(set, _NSIG/8);
}
