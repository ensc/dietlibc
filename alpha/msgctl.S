#include "syscalls.h"

syscall(msgctl,msgctl)

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
