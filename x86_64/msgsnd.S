#include "syscalls.h"

syscall(msgsnd,msgsnd)

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
