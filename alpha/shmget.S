#include "syscalls.h"

syscall(shmget,shmget)

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
