#include "syscalls.h"

_syscall2(int,socketcall,int,code,unsigned long *, args)
