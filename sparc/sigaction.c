#include <signal.h>

int __rt_sigaction(int signum, const struct sigaction *act, struct sigaction *oldact, void* restorer, long nr);

int sigaction(int signum, const struct sigaction *act, struct sigaction *oldact) {
  return __rt_sigaction(signum, act, oldact, 0, _NSIG/8);
}
