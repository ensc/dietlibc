#include "syscalls.h"

_syscall3(int,sigprocmask,int,how,void*,set,void*,oldset)
