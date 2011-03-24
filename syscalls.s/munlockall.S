#include "syscalls.h"

syscall(munlockall,munlockall)

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
