#include "syscalls.h"

syscall(shmdt,shmdt)

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
