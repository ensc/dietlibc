#include <unistd.h>
#include <errno.h>

#include <pthread.h>
#include "thread_internal.h"

int pthread_attr_setscope(pthread_attr_t *attr, int scope)
{
  __THREAD_INIT();

  if (scope==PTHREAD_SCOPE_SYSTEM) return 0;

  if (scope==PTHREAD_SCOPE_PROCESS)
    (*(__errno_location()))=ENOTSUP;
  else
    (*(__errno_location()))=EINVAL;
  return -1;
}
