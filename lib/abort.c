#include <sys/types.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>

void __stdio_flushall(void) __attribute__((weak));
void __stdio_flushall(void) { }

void abort() {
  sigset_t t;
  __stdio_flushall();
  if (!sigemptyset(&t) && !sigaddset(&t, SIGABRT))
    sigprocmask(SIG_UNBLOCK, &t, 0);
  while (1)
    if (raise(SIGABRT))
      exit(127);
}
