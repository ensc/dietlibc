#include <linux/types.h>
#include "syscalls.h"

_syscall2(int,truncate,const char*,path,off_t,length)
