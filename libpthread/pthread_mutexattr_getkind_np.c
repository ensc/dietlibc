#include <unistd.h>
#include <errno.h>

#include <pthread.h>
#include "thread_internal.h"

int pthread_mutexattr_getkind_np(const pthread_mutexattr_t *attr, int *kind)
{
  __THREAD_INIT();

  *kind=attr->__mutexkind;
  return 0;
}
