#include <unistd.h>
#include <errno.h>

#include <pthread.h>
#include "thread_internal.h"

int pthread_setcanceltype(int type, int *oldtype)
{
  _pthread_descr thread;

  __THREAD_INIT();

  if ((type==PTHREAD_CANCEL_DEFERRED) || (type==PTHREAD_CANCEL_ASYNCHRONOUS))
  {
    thread = __thread_self();

    if (oldtype) *oldtype = thread->canceltype;
    thread->canceltype = type;

    return 0;
  }

  return EINVAL;
}
