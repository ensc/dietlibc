#include "syscalls.h"

#ifdef __NR_timer_settime
syscall(timer_settime,timer_settime)
#endif

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
