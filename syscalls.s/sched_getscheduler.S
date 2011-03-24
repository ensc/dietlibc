#include "syscalls.h"

syscall(sched_getscheduler,sched_getscheduler)

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
