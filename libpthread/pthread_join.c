#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

#include "thread_internal.h"
#include <pthread.h>

int pthread_join(pthread_t th, void **thread_return)
{
  pthread_t j;
  _pthread_descr this, thread;

  __THREAD_INIT();

  this = __thread_self();
  j=__find_thread_id(th);

  if (j==-1) {
    return ESRCH;
  }

  thread = __get_thread_struct(j);

  /* error handling */
  if (thread==0) {
    return ESRCH;
  }

  if (this==thread) {
    return EDEADLK;
  }

  if (thread->detached || thread->joined) {
    return EINVAL;
  }

  return __thread_join(thread,thread_return);
}

