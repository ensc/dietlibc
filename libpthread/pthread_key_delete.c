#include <unistd.h>
#include <errno.h>

#include <pthread.h>
#include "thread_internal.h"

extern struct _thread_key __thread_keys[PTHREAD_KEYS_MAX];
int pthread_key_delete(pthread_key_t key)
{
  __THREAD_INIT();

  if (key>=PTHREAD_KEYS_MAX) {
    return EINVAL;
  }

  __thread_keys[key].used=0;

  return 0;
}

