#include "syscalls.h"

syscall(quotactl,quotactl)

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
