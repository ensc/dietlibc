#include <linux/types.h>
#include "syscalls.h"

_syscall2(int,swapon,const char*,path,int,swapflags)
