#include <linux/types.h>
#include "syscalls.h"

#ifdef __getresuid

_syscall3(int,getresuid,uid_t*,ruid,uid_t*,euid,uid_t*,suid)

#endif
