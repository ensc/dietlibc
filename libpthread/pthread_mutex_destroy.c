#include <unistd.h>
#include <errno.h>

#include <pthread.h>
#include "thread_internal.h"

int pthread_mutex_destroy(pthread_mutex_t *mutex)
{
  __THREAD_INIT();

  if ((mutex->owner)||(mutex->lock.__spinlock)) {
    return EBUSY;
  }
  return 0;
}
