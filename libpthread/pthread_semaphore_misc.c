#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#include <semaphore.h>
#include "thread_internal.h"

sem_t*sem_open(const char*name,int oflag,...) {
  (void)name; (void)oflag;
  _errno_=ENOSYS;
  return 0;
}
int sem_close(sem_t*sem) {
  (void)sem;
  _errno_=ENOSYS;
  return -1;
}
int sem_unlink(const char *name) __attribute__((alias("sem_close")));

