#include <unistd.h>
#include <errno.h>

#include <pthread.h>
#include "thread_internal.h"

int pthread_attr_setstacksize(pthread_attr_t *attr, size_t stacksize)
{
  __THREAD_INIT();

  if (stacksize>=PTHREAD_STACK_SIZE) {
    attr->__stacksize=stacksize;
    return 0;
  }
  return EINVAL;
}
