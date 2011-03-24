#include "syscalls.h"

syscall(rt_sigaction,__rt_sigaction)

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
