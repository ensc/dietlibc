#include <unistd.h>
#include <errno.h>

#include <pthread.h>
#include "thread_internal.h"

int pthread_cond_broadcast(pthread_cond_t *cond)
{
  _pthread_descr tmp;

  __THREAD_INIT();

  __NO_ASYNC_CANCEL_BEGIN;
  __pthread_lock(&(cond->lock));

  for (tmp=cond->wait_chain;tmp;tmp=tmp->waitnext) {
    tmp->waitnext=0;
    tmp->waiting=0;
  }
  cond->wait_chain=0;

  __pthread_unlock(&(cond->lock));
  __NO_ASYNC_CANCEL_STOP;
  return 0;
}

