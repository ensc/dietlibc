#include <sys/types.h>
#include "syscalls.h"

_syscall2(int,ftruncate,int,fd,off_t,length)
