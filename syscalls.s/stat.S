#include "syscalls.h"

syscall(stat,stat)

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
