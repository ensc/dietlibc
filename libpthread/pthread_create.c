#include <errno.h>

#include <pthread.h>
#include "thread_internal.h"

int pthread_create (pthread_t *thread, const pthread_attr_t *attr,
		void *(*start_routine) (void *), void *arg)
{
  int ret=0;
  unsigned int stacksize=PTHREAD_STACK_SIZE;
  char *stack = 0;

  int f_detach = 0;
  int f_inherit = PTHREAD_EXPLICIT_SCHED;
  int f_spolicy = SCHED_OTHER;
  int f_spriority = 0;

  __THREAD_INIT();

  if (attr) {
    f_detach	= attr->__detachstate;
    f_spolicy	= attr->__schedpolicy;
    f_spriority	= attr->__schedparam.sched_priority;
    f_inherit	= attr->__inheritsched;
    stack	= attr->__stackaddr;
    stacksize	= attr->__stacksize;
    if ((f_spolicy!=SCHED_OTHER)&&(f_spriority==0)) {
      (*(__errno_location()))=EINVAL;
      return -1;
    }
  }

  ret = __thread_create(start_routine, arg, stack, stacksize, f_detach,
			f_inherit, f_spolicy, f_spriority);

  if (ret!=-1) (*thread)=ret;
  return ret;
}
