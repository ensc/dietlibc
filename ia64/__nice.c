#include <sys/resource.h>

int nice(int x) {
  if (setpriority(PRIO_PROCESS,0,x)) return -1;
  return getpriority(PRIO_PROCESS,0);
}
