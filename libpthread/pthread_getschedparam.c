#include <unistd.h>
#include <errno.h>

#include <pthread.h>
#include "thread_internal.h"

int pthread_getschedparam(pthread_t target_thread, int *policy, struct sched_param *param)
{
  int p;
  __THREAD_INIT();

  if (__find_thread_id(target_thread)<0) {
    return ESRCH;
  }

  if (((p=sched_getscheduler(target_thread))==-1)||
      (sched_getparam(target_thread,param) ==-1)) {
    return (*(__errno_location()));
  }
  *policy=p;
  return 0;
}
