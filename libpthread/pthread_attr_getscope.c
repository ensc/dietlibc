#include <unistd.h>
#include <errno.h>

#include <pthread.h>
#include "thread_internal.h"

int pthread_attr_getscope(const pthread_attr_t*attr,int*scope) {
  (void)attr;
  *scope=PTHREAD_SCOPE_SYSTEM;
  return 0;
}
