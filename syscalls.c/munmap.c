#include <linux/types.h>
#include "syscalls.h"

_syscall2(int,munmap,void*,start,size_t,len)

