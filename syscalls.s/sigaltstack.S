#include "syscalls.h"

.text
.weak sigaltstack
sigaltstack:
syscall(sigaltstack,__sigaltstack)
