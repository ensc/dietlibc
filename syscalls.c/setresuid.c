#include <linux/types.h>
#include "syscalls.h"

#ifdef __NR_setresuid

_syscall3(int,setresuid,uid_t*,ruid,uid_t*,euid,uid_t*,suid)

#endif
