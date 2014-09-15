#include "syscalls.h"

#ifdef __NR_inotify_init1
syscall(inotify_init1,inotify_init1)
#endif
