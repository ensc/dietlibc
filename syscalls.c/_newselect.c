#include <linux/types.h>
#include <linux/time.h>
#include "syscalls.h"

_syscall5(int,_newselect,int,n,fd_set *,rd,fd_set *,wr,fd_set *,ex,struct timeval *,timeval);

