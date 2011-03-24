#include "syscalls.h"

#ifdef __NR_timer_delete
syscall(timer_delete,timer_delete)
#endif

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
