#include "syscalls.h"

syscall(clock_getres,clock_getres)

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
