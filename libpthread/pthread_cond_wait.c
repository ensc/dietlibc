#include <unistd.h>
#include <errno.h>

#include <pthread.h>
#include "thread_internal.h"

int pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex)
{
  _pthread_descr tmp;
  _pthread_descr this;

  __THREAD_INIT();

  this=__thread_self();

  /* put in wait-chain */
  __pthread_lock(&(cond->lock));
  this->waiting=1;
  if (cond->wait_chain) {
    for(tmp=cond->wait_chain;tmp->waitnext;tmp=tmp->waitnext);
    tmp->waitnext=this;
  } else cond->wait_chain=this;
  __pthread_unlock(&(cond->lock));

  /* Aeh yeah / wait till signal */
  pthread_mutex_unlock(mutex);
  while (this->waiting) {
    __thread_wait_some_time();
    if (this->canceled) this->waiting=0;	/* we got a cancel signal */
  }
  pthread_mutex_lock(mutex);

  __TEST_CANCEL();
  return 0;
}


