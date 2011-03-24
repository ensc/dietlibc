#include "syscalls.h"

syscall(chmod,chmod)

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
