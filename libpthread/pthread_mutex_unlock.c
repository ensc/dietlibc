#include <unistd.h>
#include <sched.h>
#include <errno.h>

#include <pthread.h>
#include "thread_internal.h"

/* will never return EINVAL ! */

int pthread_mutex_unlock(pthread_mutex_t*mutex) {
  _pthread_descr this=__thread_self();
  if (mutex->owner==this) {
    if (mutex->kind==PTHREAD_MUTEX_RECURSIVE_NP) {
      if (--(mutex->count)) return 0;
    }
    mutex->owner=0;
    UNLOCK(mutex);
  }
  else if (mutex->kind==PTHREAD_MUTEX_ERRORCHECK_NP) {
    return EPERM;
  }
  return 0;
}
