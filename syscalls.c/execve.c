#include "syscalls.h"

_syscall3(int,execve,const char *,fn,void *,argv,void *,envp)

