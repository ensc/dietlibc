#include <signal.h>

extern void rt_sigreturn() __attribute__((noreturn));
extern void sigreturn() __attribute__((noreturn));

int __rt_sigaction(int signum, const struct sigaction *act, struct sigaction *oldact, long nr, void* restorer);

int __libc_sigaction(int signum, const struct sigaction *act, struct sigaction *oldact);
int __libc_sigaction(int signum, const struct sigaction *act, struct sigaction *oldact) {
  void *restorer = (act && act->sa_flags & SA_SIGINFO) ? (void *)rt_sigreturn : (void *)sigreturn;
  return __rt_sigaction(signum, act, oldact, _NSIG/8, restorer);
}
int sigaction(int signum, const struct sigaction *act, struct sigaction *oldact) 
__attribute__((weak,alias("__libc_sigaction")));
