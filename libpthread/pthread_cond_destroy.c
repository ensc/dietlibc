#include <unistd.h>
#include <errno.h>

#include <pthread.h>
#include "thread_internal.h"

int pthread_cond_destroy(pthread_cond_t *cond)
{
  __THREAD_INIT();

  if (cond->wait_chain) {
    return EBUSY;
  }

  memset(cond,0,sizeof(pthread_cond_t));
  return 0;
}

