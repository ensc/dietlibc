#include "syscalls.h"

syscall(brk,__diet_brk)

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
