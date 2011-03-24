#include "syscalls.h"

#ifdef __NR_timer_create
syscall(timer_create,timer_create)
#endif

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
