/* signal() emulation for alpha */
#include <signal.h>
#include <errno.h>

sighandler_t signal(int signum, sighandler_t action) {
  int err;
  sigset_t block_mask;
  struct sigaction newact, oldact;
  sigemptyset (&block_mask);
  newact.sa_handler = action;
  newact.sa_mask = block_mask;
  newact.sa_flags = SA_RESTART;
  err = sigaction (signum, &newact, &oldact);
  if (err)
    return SIG_ERR;
  else
    return oldact.sa_handler;
}
