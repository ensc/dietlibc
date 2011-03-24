#include "syscalls.h"

syscall(clock_nanosleep,clock_nanosleep)

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
