//#include <unistd.h>

#include <pthread.h>
#include "thread_internal.h"

int open(const char *pathname, int flags, mode_t mode)
{
  __TEST_CANCEL();
  return __libc_open(pathname,flags,mode);
}
