#include <linux/types.h>
#include "syscalls.h"

_syscall2(int,setregid,gid_t,rgid,gid_t,egid)
