#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sched.h>
#include <sys/resource.h>

#include <pthread.h>
#include "thread_internal.h"

/*
 * CLONE_VM		use same memory
 * CLONE_FS		use same filesystemposition
 * CLONE_FILES		use same file descriptors
 * CLONE_SIGHAND	all cloned processes have the same signal handler
 */

static void *__thread_starter(void *arg)
{
  _pthread_descr td = (_pthread_descr)arg;

  /* limit stack so that we NEVER have to manualy guard it ! */
  setrlimit(RLIMIT_STACK, (struct rlimit *)&(td->stack_size));

  /* set scheduler */
  if (td->policy!=SCHED_OTHER) {
    struct sched_param sp;
    sp.sched_priority=td->priority;
    sched_setscheduler(getpid(),td->policy, &sp);
  }

#if DEBUG
  printf("in starter %d, parameter %8p\n", td->pid, td->func);
#endif

  if (!td->canceled)
  {
    td->retval=td->func(td->arg);
  }

  return td->retval;
}

int __thread_create(void *(*__start_routine) (void *), void *__arg,
		char* stack, unsigned long stacksize,
		int detach,
		int inherit, int spolicy, int spriority)
{
  _pthread_descr td;
  int ret=0;

  __THREAD_INIT();

  td = __thread_get_free();

  if (stack)
    td->userstack=1;
  else {
    stack=(char*)malloc(stacksize);
    stack+=stacksize;
  }

  td->func		= __start_routine;
  td->arg		= __arg;
  td->stack_begin	= stack;
  td->stack_size	= stacksize;
  td->detached		= detach;
  if (inherit==PTHREAD_INHERIT_SCHED) {
    _pthread_descr this = __thread_self();
    td->policy		= this->policy;
    td->priority	= this->priority;
  } else {
    td->policy		= spolicy;
    td->priority	= spriority;
  }

  ret = __clone(__thread_starter, stack, CLONE_VM | CLONE_FS | CLONE_FILES |
		CLONE_SIGHAND | SIGCHLD, td);

  if (ret>0) td->pid=ret;

  return ret;
}
