#include "syscalls.h"

_syscall5(int,mount,const char *,spec,const char *,dir,const char *,type,unsigned long,rwflag,const void *,data);

