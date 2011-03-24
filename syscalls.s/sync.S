#include "syscalls.h"

syscall(sync,sync)

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
