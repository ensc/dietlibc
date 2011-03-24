#include "syscalls.h"

syscall(sched_rr_get_interval,sched_rr_get_interval)

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
