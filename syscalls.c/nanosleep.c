#include <linux/types.h>
#include <linux/time.h>
#include "syscalls.h"

_syscall2(int,nanosleep,struct timeval *,in,struct timeval *,out)
