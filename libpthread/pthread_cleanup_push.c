#include <unistd.h>
#include <errno.h>

#include <pthread.h>
#include "thread_internal.h"

void pthread_cleanup_push(void (*func)(void*), void *arg)
{
  _pthread_descr this;
  int i;

  __THREAD_INIT();

  this = __thread_self();

  for (i=0; i<PTHREAD_MAX_CLEANUP; ++i) {
    if (this->cleanup_stack[i].func==0) {
      this->cleanup_stack[i].func=func;
      this->cleanup_stack[i].arg=arg;
      break;
    }
  }
}
