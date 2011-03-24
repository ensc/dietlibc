#include "syscalls.h"

syscall(fstat,fstat)

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
