#include <linux/types.h>
#include "syscalls.h"

_syscall3(int,mprotect,const void *,addr, size_t, len, int, prot)
