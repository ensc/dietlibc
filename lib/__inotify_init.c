#include <sys/inotify.h>
#include "syscalls.h"

#if defined(__NR_inotify_init1) && !defined(__NR_inotify_init)
int inotify_init() {
  return inotify_init1(0);
}
#endif
