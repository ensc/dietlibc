#include "syscalls.h"

syscall(clock_settime,clock_settime)

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
