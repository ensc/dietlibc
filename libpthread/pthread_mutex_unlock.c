#include <unistd.h>
#include <sched.h>
#include <errno.h>

#include <pthread.h>
#include "thread_internal.h"

/* will never return EINVAL ! */

int pthread_mutex_unlock(pthread_mutex_t *mutex)
{
  _pthread_descr this;

  __THREAD_INIT();

  this = __thread_self();

  if (this==mutex->owner) {
    if (mutex->kind==PTHREAD_MUTEX_RECURSIVE_NP) {
      if (--(mutex->count))
	return 0;
    }

    mutex->owner=0;
    __pthread_unlock(&(mutex->lock));
  }
  else if (mutex->kind==PTHREAD_MUTEX_ERRORCHECK_NP) {
    return EPERM;
  }

  return 0;
}
