#include "syscalls.h"

syscall(getppid,getppid)

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
