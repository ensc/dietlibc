#include "syscalls.h"

syscall(sched_get_priority_max,sched_get_priority_max)

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
