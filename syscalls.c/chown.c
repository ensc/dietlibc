#include <linux/types.h>
#include "syscalls.h"

_syscall3(int,chown,char*,path,uid_t,owner,gid_t,group)

