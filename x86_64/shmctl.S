#include "syscalls.h"

syscall(shmctl,shmctl)

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
