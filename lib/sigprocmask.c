#include <signal.h>

int sigprocmask(int how, const sigset_t *set, sigset_t *oldset) {
  return __rt_sigprocmask(how, set, oldset, _NSIG/8);
}
