#include "syscalls.h"

syscall(keyctl,keyctl)

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
