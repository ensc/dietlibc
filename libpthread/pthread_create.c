#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

#include <pthread.h>
#include "thread_internal.h"

int pthread_create (pthread_t *thread, const pthread_attr_t *attr,
		void *(*start_routine) (void *), void *arg)
{
  int ret=0;
  _pthread_descr td;
  pthread_attr_t default_attr;

  __THREAD_INIT();

  if (start_routine==0) {
    return EINVAL;
  }

  td = __thread_get_free();

  if (td) {
    td->go.__spinlock=1;
    if (!(attr)) {
      pthread_attr_init(&default_attr);
      attr=&default_attr;
    }

    if ((td->policy!=SCHED_OTHER)&&(td->priority==0)) {
      return EINVAL;
    }

    if (attr->__inheritsched==PTHREAD_INHERIT_SCHED) {
      _pthread_descr this = __thread_self();
      td->policy	= this->policy;
      td->priority	= this->priority;
    } else {
      td->policy	= attr->__schedpolicy;
      td->priority	= attr->__schedparam.sched_priority;
    }

    td->func		= start_routine;
    td->arg		= arg;

    td->detached	= attr->__detachstate;

    td->stack_size	= attr->__stacksize;

    if (!(td->stack_addr)) {
      char *stack=(char*)malloc(td->stack_size);
      if (!(stack)) {
	return EINVAL;
      }
      td->stack_begin	= stack;
      td->stack_addr	= stack+td->stack_size;
    } else {
      td->stack_begin	= 0;
      td->stack_addr	= attr->__stackaddr;
    }

    ret = signal_manager_thread(td);

    if (ret>1)
      *thread=ret;
    else {
      ++td->exited;	/* mark as exited */
      __thread_cleanup(td);
      if (ret<0) return (*(__errno_location()));
      return EAGAIN;
    }
  }
  else
    return EAGAIN;

  return 0;
}
