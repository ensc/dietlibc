#include <linux/types.h>
#include "syscalls.h"

_syscall3(int,poll,void *,ufds,int,nfds,int,timeout)
