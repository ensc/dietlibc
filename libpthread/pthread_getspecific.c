#include <unistd.h>
#include <errno.h>

#include <pthread.h>
#include "thread_internal.h"

extern struct _thread_key __thread_keys[PTHREAD_KEYS_MAX];
const void *pthread_getspecific(pthread_key_t key)
{
  int id;
  __THREAD_INIT();

  id=__find_thread_id(getpid());

  if ((key<PTHREAD_KEYS_MAX) && (__thread_keys[key].used)) {
    return __thread_keys[key].tkd[id];
  }
  return 0;
}

