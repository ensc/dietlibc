#include <unistd.h>
#include <errno.h>

#include <pthread.h>
#include "thread_internal.h"

int pthread_setcancelstate(int state, int *oldstate)
{
  _pthread_descr thread;

  __THREAD_INIT();

  if ((state==PTHREAD_CANCEL_ENABLE) || (state==PTHREAD_CANCEL_DISABLE))
  {
    thread = __thread_self();

    if (oldstate) *oldstate = thread->cancelstate;
    thread->cancelstate = state;

    return 0;
  }

  return EINVAL;
}
