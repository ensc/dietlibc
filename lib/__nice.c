#include "syscalls.h"
#include <errno.h>
#include <sys/resource.h>

#ifndef __NR_nice
int nice(int i) {
  if (setpriority(PRIO_PROCESS,0,getpriority(PRIO_PROCESS,0)+i) == -1) {
    errno=EPERM;
    return -1;
  }
  return getpriority(PRIO_PROCESS,0);
}
#endif
