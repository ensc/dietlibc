#include "syscalls.h"

syscall(clock_gettime,clock_gettime)

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
