#include <unistd.h>
#include <errno.h>

#include <pthread.h>
#include "thread_internal.h"

void pthread_exit(void *retval)
{
  _pthread_descr this;

  __THREAD_INIT();

  this = __thread_self();
  this->retval = retval;

  longjmp(this->jmp_exit,1);
}
