#include <errno.h>

#include <pthread.h>
#include "thread_internal.h"

int pthread_detach(pthread_t th)
{
  pthread_t j;
  _pthread_descr thread;

  __THREAD_INIT();

  j=__find_thread_id(th);

  if (j==-1) {
    (*(__errno_location()))=ESRCH;
    return -1;
  }
  thread = __get_thread_struct(j);

  if (thread==0) {
    (*(__errno_location()))=ESRCH;
    return -1;
  }

  if (thread->detached) {
    (*(__errno_location()))=EINVAL;
    return -1;
  }

  if (!thread->joined) {
    thread->detached=1;
  }

  return 0;
}
