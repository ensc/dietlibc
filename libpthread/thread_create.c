#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sched.h>

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

#if DEBUG
  printf("in starter %d, parameter %8p\n", td->pid, td->func);
#endif

  if (!td->canceled)
  {
    td->retval=td->func(td->arg);
  }

#if DEBUG
  printf("in starter %d, exited %8p\n", td->pid, td->retval);
#endif

  return td->retval;
}

int __thread_create(void *(*__start_routine) (void *), void *__arg,
		    char* stack, unsigned long stacksize)
{
  _pthread_descr td;
  int ret=0;

  __THREAD_INIT();

  td = __thread_get_free();

  if (stack)
    td->userstack=1;
  else
    stack=(char*)malloc(stacksize);

  td->func		= __start_routine;
  td->arg		= __arg;
  td->stack_begin	= stack;

  stack+=stacksize;

  ret = __clone(__thread_starter, stack, CLONE_VM | CLONE_FS | CLONE_FILES |
		CLONE_SIGHAND | SIGCHLD, td);

  if (ret>0) td->pid=ret;

  return ret;
}
