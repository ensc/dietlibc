#include <sys/types.h>
#include <signal.h>
#include <stdlib.h>

void abort() {
  sigset_t t;
  if (!sigemptyset(&t) && !sigaddset(&t, SIGABRT))
    sigprocmask(SIG_UNBLOCK, &t, 0);
  while (1)
    if (raise(SIGABRT))
      exit(127);
}
