#include <linux/types.h>
#include "syscalls.h"

_syscall2(int,access,const char*,pathname,int,mode)
