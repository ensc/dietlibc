#include <unistd.h>
#include <errno.h>

#include <pthread.h>
#include "thread_internal.h"

int pthread_setschedparam(pthread_t target_thread, int policy, const struct sched_param *param)
{
  __THREAD_INIT();

  if (__find_thread_id(target_thread)<0) {
    return ESRCH;
  }

  if (((policy == SCHED_OTHER) && (param->sched_priority==0)) ||
      (((policy == SCHED_RR) || (policy == SCHED_FIFO)) &&
       ((param->sched_priority > 0) && (param->sched_priority <100))))
    return sched_setscheduler(target_thread, policy, param);

  return EINVAL;
}
