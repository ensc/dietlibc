#include <linux/types.h>
#include "syscalls.h"

_syscall2(int,kill,pid_t,pid,int,sig)
