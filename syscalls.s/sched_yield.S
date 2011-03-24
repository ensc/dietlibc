#include "syscalls.h"

syscall(sched_yield,sched_yield)

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
