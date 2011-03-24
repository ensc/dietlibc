#include "syscalls.h"

#ifdef __NR_timer_getoverrun
syscall(timer_getoverrun,timer_getoverrun)
#endif

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
