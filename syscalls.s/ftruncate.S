#include "syscalls.h"

syscall(ftruncate,ftruncate)

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
