#include <unistd.h>
#include <errno.h>

#include <pthread.h>
#include "thread_internal.h"

void pthread_cleanup_pop(int execute)
{
  _pthread_descr this;
  struct thread_cleanup_t tmp;
  int i=PTHREAD_MAX_CLEANUP;

  __THREAD_INIT();

  this = __thread_self();

  while(i>0) {
    if (this->cleanup_stack[--i].func) {
      tmp.func=this->cleanup_stack[i].func;
      tmp.arg=this->cleanup_stack[i].arg;
      this->cleanup_stack[i].func=0;
      break;
    }
  }

  if (execute) tmp.func(tmp.arg);
}
