#include <unistd.h>
#include <errno.h>

#include <pthread.h>
#include "thread_internal.h"

int pthread_cancel(pthread_t th)
{
  pthread_t j;
  _pthread_descr thread;

  __THREAD_INIT();

  j=__find_thread_id(th);

  if (j==-1) {
    return ESRCH;
  }
  thread = __get_thread_struct(j);

  if (thread==0) {
    return ESRCH;
  }

  if (thread->cancelstate!=PTHREAD_CANCEL_DISABLE) {
    thread->canceled=1;
    if (thread->canceltype!=PTHREAD_CANCEL_DEFERRED) {
      kill(thread->pid,SIGHUP);
    }
  }

  return 0;
}
