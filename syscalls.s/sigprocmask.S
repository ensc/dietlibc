#include "syscalls.h"

.text
.weak sigprocmask
sigprocmask:
syscall(sigprocmask,__sigprocmask)
