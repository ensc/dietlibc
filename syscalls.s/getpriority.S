#include "syscalls.h"

syscall(getpriority,getpriority)

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
