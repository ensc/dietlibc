#include <linux/types.h>
#include "syscalls.h"

_syscall2(int,setreuid,uid_t,ruid,uid_t,euid)
