#include <linux/types.h>
#include <linux/time.h>
#include "syscalls.h"

_syscall2(int,gettimeofday,struct timeval *,tv,void *,tz)
