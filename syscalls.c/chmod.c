#include <linux/types.h>
#include "syscalls.h"

_syscall2(int,chmod,const char * ,path,mode_t,mode)

