#include "syscalls.h"

.text
.align 2
.weak sigprocmask
sigprocmask:
syscall(osf_sigprocmask,__sigprocmask)
