#include <unistd.h>
#include <errno.h>

#include <pthread.h>
#include "thread_internal.h"

int pthread_cond_signal(pthread_cond_t *cond)
{
  _pthread_descr tmp=0;

  __THREAD_INIT();

  __pthread_lock(&(cond->lock));
  if ((tmp=cond->wait_chain)) cond->wait_chain=tmp->waitnext;
  __pthread_unlock(&(cond->lock));

  if (tmp) {
    tmp->waitnext=0;
    tmp->waiting=0;
  }

  return 0;
}

