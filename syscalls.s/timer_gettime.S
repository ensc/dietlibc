#include "syscalls.h"

#ifdef __NR_timer_gettime
syscall(timer_gettime,timer_gettime)
#endif

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
