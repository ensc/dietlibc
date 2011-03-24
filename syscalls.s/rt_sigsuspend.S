#include "syscalls.h"

syscall(rt_sigsuspend,__rt_sigsuspend)

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
