#include <unistd.h>
#include <errno.h>

#include <pthread.h>
#include "thread_internal.h"

int pthread_mutex_init(pthread_mutex_t *mutex,
			const pthread_mutexattr_t *mutexattr)
{
  __THREAD_INIT();

  memset(mutex,0,sizeof(pthread_mutex_t));
  if (mutexattr) {
    mutex->kind=mutexattr->__mutexkind;
  }
  return 0;
}

