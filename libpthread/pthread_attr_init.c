#include <unistd.h>
#include <errno.h>

#include <pthread.h>
#include "thread_internal.h"

int pthread_attr_init(pthread_attr_t *attr)
{
  memset(attr,0,sizeof(pthread_attr_t));
  return 0;
}

int pthread_attr_destroy(pthread_attr_t *attr) __attribute__((alias("pthread_attr_init")));

