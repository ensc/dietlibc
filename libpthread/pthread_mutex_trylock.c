#include <unistd.h>
#include <sched.h>
#include <errno.h>

#include <pthread.h>
#include "thread_internal.h"

/* will never return EINVAL ! */

int pthread_mutex_trylock(pthread_mutex_t *mutex)
{
  _pthread_descr this;

  __THREAD_INIT();

  this = __thread_self();

  if (this!=mutex->owner) {
    /* wait for mutex to free */
    if (__pthread_trylock(&(mutex->lock))) {
      return EBUSY;
    }

    mutex->owner=this;
  }
  else if (mutex->kind==PTHREAD_MUTEX_ERRORCHECK_NP) {
    return EDEADLK;
  }

  if (mutex->kind==PTHREAD_MUTEX_RECURSIVE_NP) ++(mutex->count);

  return 0;
}
