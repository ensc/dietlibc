#include "syscalls.h"

syscall(lstat,lstat)

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
