#include "syscalls.h"

syscall(sched_setaffinity,sched_setaffinity)


#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
