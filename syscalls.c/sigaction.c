#include "syscalls.h"

_syscall3(int,sigaction,int,signum,void*,act,void*,oldact)
