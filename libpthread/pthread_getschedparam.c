#include <unistd.h>
#include <errno.h>

#include <pthread.h>
#include "thread_internal.h"

int pthread_getschedparam(pthread_t target_thread, int *policy, const struct sched_param *param)
{
  __THREAD_INIT();

  if (__find_thread_id(target_thread)<0) {
    (*(__errno_location()))=ESRCH;
    return -1;
  }

  if ((p=sched_getscheduler(target_thread))!=-1) {
    *policy=p;
    return sched_getparam(target_thread,param);
  }
  return -1;
}
