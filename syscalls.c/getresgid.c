#include <linux/types.h>
#include "syscalls.h"

#ifdef __NR_getresgid

_syscall3(int,getresgid,gid_t*,rgid,gid_t*,egid,gid_t*,sgid)

#endif
