#include "syscalls.h"

syscall(getrusage,getrusage)

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
