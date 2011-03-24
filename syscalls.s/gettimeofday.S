#include "syscalls.h"

syscall(gettimeofday,gettimeofday)

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
