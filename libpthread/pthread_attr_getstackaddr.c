#include <unistd.h>
#include <errno.h>

#include <pthread.h>
#include "thread_internal.h"

int pthread_attr_getstackaddr(pthread_attr_t *attr, void **stack)
{
  __THREAD_INIT();

  *stack=attr->__stackaddr;
  return 0;
}
