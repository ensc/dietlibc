#include "syscalls.h"

syscall(sched_getparam,sched_getparam)

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
