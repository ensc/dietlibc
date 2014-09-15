#define fcntl lib_fcntl
#include <unistd.h>

#include <pthread.h>
#include "thread_internal.h"
#undef fcntl

int fcntl(int fd, int cmd, void *arg);
int fcntl(int fd, int cmd, void *arg) {
  __TEST_CANCEL();
  return __libc_fcntl(fd,cmd,arg);
}
