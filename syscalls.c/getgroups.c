#include <linux/types.h>
#include "syscalls.h"

_syscall2(int,getgroups,int,size,int*,list)
