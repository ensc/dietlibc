#include "syscalls.h"

syscall(pwrite,__pwrite)

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
