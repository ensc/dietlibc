#include <errno.h>

#include <pthread.h>
#include "thread_internal.h"

void pthread_testcancel()
{
  _pthread_descr thread;
  __THREAD_INIT();

  thread=__thread_self();

  if (thread && thread->canceled) {
    pthread_exit(PTHREAD_CANCELED);
  }
}
