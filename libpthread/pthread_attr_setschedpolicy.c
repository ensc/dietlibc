#include <unistd.h>
#include <errno.h>

#include <pthread.h>
#include "thread_internal.h"

int pthread_attr_setschedpolicy(pthread_attr_t *attr, int policy)
{
  if (policy==SCHED_OTHER) {
    attr->__schedpolicy=policy;
    return 0;
  }
  if ((policy==SCHED_FIFO) || (policy==SCHED_RR)) {
    if (geteuid()==0) {
      attr->__schedpolicy=policy;
      return 0;
    }
    (*(__errno_location()))=ENOTSUP;
  }
  else
    (*(__errno_location()))=EINVAL;
  return -1;
}
