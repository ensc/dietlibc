#include <linux/types.h>
#include "syscalls.h"

_syscall2(int,mkdir,void*,path,mode_t,mode)
