#include <pthread.h>
#include "thread_internal.h"

int pthread_create (pthread_t *__thread, const pthread_attr_t *__attr,
		void *(*__start_routine) (void *), void *__arg)
{
  int ret=0;
  unsigned int stacksize=16*1024;
  char *stack = 0;

  ret = __thread_create(__start_routine, __arg, stack, stacksize);

  if (ret!=-1) (*__thread)=ret;
  return ret;
}
