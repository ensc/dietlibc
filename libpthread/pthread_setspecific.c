#include <unistd.h>
#include <errno.h>

#include <pthread.h>
#include "thread_internal.h"

extern struct _thread_key __thread_keys[PTHREAD_KEYS_MAX];
int pthread_setspecific(pthread_key_t key, const void *value)
{
  int id;
  __THREAD_INIT();

  id=__find_thread_id(getpid());
  if ((key<PTHREAD_KEYS_MAX) && (__thread_keys[key].used)) {
    __thread_keys[key].tkd[id]=value;
    return 0;
  }
  return EINVAL;
}

