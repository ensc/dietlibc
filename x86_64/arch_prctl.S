#include "syscalls.h"

syscall(arch_prctl,arch_prctl)

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
