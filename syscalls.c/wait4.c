#include <linux/types.h>
#include "syscalls.h"

_syscall4(int,wait4,pid_t,pid,int *,status,int,opts,void *,rusage)
