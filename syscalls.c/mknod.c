#include <linux/types.h>
#include "syscalls.h"

_syscall3(int,mknod,const char*,pathname,mode_t,mode,dev_t,dev)

