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
    (*(__errno_location()))=ESRCH;
    return -1;
  }

  thread = __get_thread_struct(j);

  /* error handling */
  if (thread==0) {
    (*(__errno_location()))=ESRCH;
    return -1;
  }

  if (this==thread) {
    (*(__errno_location()))=EDEADLK;
    return -1;
  }

  if (thread->detached || thread->joined) {
    (*(__errno_location()))=EINVAL;
    return -1;
  }

  thread->joined=this;
  this->join=1;

  while(this->join) __thread_wait_some_time();

  if (thread_return) *thread_return=this->retval;

  return 0;
}

