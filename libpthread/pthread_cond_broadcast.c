#include <unistd.h>
#include <errno.h>

#include <pthread.h>
#include "thread_internal.h"

int pthread_cond_broadcast(pthread_cond_t *cond)
{
  _pthread_descr tmp;

  __THREAD_INIT();

  __pthread_lock(&(cond->lock));
  while ((tmp=cond->wait_chain)) {
    cond->wait_chain=tmp->waitnext;
    tmp->waitnext=0;
    tmp->waiting=0;
  }
  __pthread_unlock(&(cond->lock));

  return 0;
}

