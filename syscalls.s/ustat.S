#include "syscalls.h"

syscall(ustat,ustat)

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
