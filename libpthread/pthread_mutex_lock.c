#include <unistd.h>
#include <sched.h>
#include <errno.h>

#include <pthread.h>
#include "thread_internal.h"

/* will never return EINVAL ! */

int pthread_mutex_lock(pthread_mutex_t*mutex) {
  _pthread_descr this=__thread_self();
  if (mutex->owner!=this) {
    /* wait for mutex to free */
    LOCK(mutex);
    mutex->owner=this;
    mutex->count=0;
  }
  else if (mutex->kind==PTHREAD_MUTEX_ERRORCHECK_NP) return EDEADLK;
  if (mutex->kind==PTHREAD_MUTEX_RECURSIVE_NP) ++(mutex->count);
  return 0;
}
