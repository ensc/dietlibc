#include <unistd.h>

#include <pthread.h>
#include "thread_internal.h"

static pthread_mutex_t mutex_alloc = PTHREAD_MUTEX_INITIALIZER;

void free(void *ptr)
{
  int oldtype;
  pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, &oldtype);
  pthread_mutex_lock(&mutex_alloc);
  __libc_free(ptr);
  pthread_mutex_unlock(&mutex_alloc);
  pthread_setcanceltype(oldtype,0);
  pthread_testcancel();
}

void *malloc(size_t size)
{
  int oldtype;
  register void *ret;
  pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, &oldtype);
  pthread_mutex_lock(&mutex_alloc);
  ret=__libc_malloc(size);
  pthread_mutex_unlock(&mutex_alloc);
  pthread_setcanceltype(oldtype,0);
  pthread_testcancel();
  return ret;
}
