#include "syscalls.h"

syscall(sched_getaffinity,__syscall_sched_getaffinity)


#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
