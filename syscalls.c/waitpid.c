#include <linux/types.h>
#include "syscalls.h"

_syscall3(int,waitpid,int,pid,int *,status,int,options)
