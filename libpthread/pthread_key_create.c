#include <unistd.h>
#include <errno.h>

#include <pthread.h>
#include "thread_internal.h"

extern struct _pthread_fastlock __thread_keys_lock;
extern struct _thread_key __thread_keys[PTHREAD_KEYS_MAX];
int pthread_key_create(pthread_key_t *key, void (*destructor)(const void*))
{
  int ret=-1,i;

  __THREAD_INIT();

  __NO_ASYNC_CANCEL_BEGIN;
  __pthread_lock(&__thread_keys_lock);

  for (i=0; i<PTHREAD_KEYS_MAX; i++) {
    if (!__thread_keys[i].used) {
      __thread_keys[i].used=1;
      __thread_keys[i].destructor=destructor;
      memset(__thread_keys[i].tkd,0,PTHREAD_THREADS_MAX*sizeof(void*));
      *key=i;
      ret=0;
      break;
    }
  }

  __pthread_unlock(&__thread_keys_lock);
  __NO_ASYNC_CANCEL_END;

  if (ret) return EAGAIN;
  return 0;
}

