#include <linux/types.h>
#include "syscalls.h"

#ifdef __NR_setresgid

_syscall3(int,setresgid,gid_t*,rgid,gid_t*,egid,gid_t*,sgid)

#endif
