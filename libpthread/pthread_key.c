#include <unistd.h>
#include <errno.h>

#include <pthread.h>
#include "thread_internal.h"

struct _pthread_fastlock __thread_keys_lock;
struct _thread_key __thread_keys[PTHREAD_KEYS_MAX];

void __thread_start__key(int id);
void __thread_exit__key(int id);

void __thread_start__key(int id)
{
  int i;

  if (id<2) return;

  __NO_ASYNC_CANCEL_BEGIN;
  __pthread_lock(&__thread_keys_lock);

  for (i=0; i<PTHREAD_KEYS_MAX; i++) {
    __thread_keys[i].tkd[id]=0;
  }

  __pthread_unlock(&__thread_keys_lock);
  __NO_ASYNC_CANCEL_END;
}

void __thread_exit__key(int id)
{
  int i,try;

  if (id<2) return;

  __NO_ASYNC_CANCEL_BEGIN;
//  __pthread_lock(&__thread_keys_lock);

  for (i=0; i<PTHREAD_KEYS_MAX; i++) {
    if ((__thread_keys[i].used) && (__thread_keys[i].destructor)) {
      for (try=0;__thread_keys[i].tkd[id] && (try<PTHREAD_DESTRUCTOR_ITERATIONS);++try)
	__thread_keys[i].destructor(__thread_keys[i].tkd[id]);
    }
  }

//  __pthread_unlock(&__thread_keys_lock);
  __NO_ASYNC_CANCEL_STOP;
}
