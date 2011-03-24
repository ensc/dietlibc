#include "syscalls.h"

syscall(dup2,dup2)

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
