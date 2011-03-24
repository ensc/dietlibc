#include "syscalls.h"

syscall(munmap,munmap)

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
