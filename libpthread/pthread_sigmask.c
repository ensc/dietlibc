#include <signal.h>
#include <errno.h>

#include <pthread.h>

int pthread_sigmask(int how, const sigset_t*newset, sigset_t *oldset) {
  return (sigprocmask(how,newset,oldset)==-1)?(*(__errno_location())):0;
}
