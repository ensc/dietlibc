#include <linux/types.h>
#include "syscalls.h"

_syscall4(int,mremap,void*,old_address,size_t,old_size,size_t,new_size,unsigned long,flags)

