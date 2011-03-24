#include "syscalls.h"

syscall(sched_setparam,sched_setparam)

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
