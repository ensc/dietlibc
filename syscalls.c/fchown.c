#include <linux/types.h>
#include "syscalls.h"

_syscall3(int,fchown,int,fd,uid_t,owner,gid_t,group)

