#include <signal.h>

int sigqueueinfo(int pid, int sig, siginfo_t *info) {
  return __rt_sigqueueinfo(pid, sig, info);
}
