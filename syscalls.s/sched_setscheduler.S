#include "syscalls.h"

syscall(sched_setscheduler,sched_setscheduler)

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
