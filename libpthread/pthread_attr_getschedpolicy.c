#include <unistd.h>
#include <errno.h>

#include <pthread.h>
#include "thread_internal.h"

int pthread_attr_getschedpolicy(const pthread_attr_t *attr, int *policy)
{
  __THREAD_INIT();

  *policy = attr->__schedpolicy;
  return 0;
}
