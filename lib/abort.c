#include <linux/types.h>
#include <linux/signal.h>
#include <signal.h>

extern int exit(int code) __attribute__ ((noreturn));

void abort() {
  sigset_t t;
  if (!sigemptyset(&t) && !sigaddset(&t, SIGABRT))
    sigprocmask(SIG_UNBLOCK, &t, 0);
  while (1)
    if (raise(SIGABRT))
      exit(127);
}
