#include <linux/types.h>
#include "syscalls.h"

_syscall3(int,open,const char *,fn,int,flags,mode_t,mode)
