#include <unistd.h>
#include <sys/types.h>
#include "syscalls.h"

_syscall5(int,_llseek,uint,fd,ulong,hi,ulong,lo,loff_t*,res,uint,wh);
