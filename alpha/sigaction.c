#include <signal.h>

int sigaction(int signum, const struct sigaction *act, struct sigaction *oldact) {
  return __rt_sigaction(signum, act, oldact, _NSIG/8, 0);
}
