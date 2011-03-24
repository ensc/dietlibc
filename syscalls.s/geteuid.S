#include "syscalls.h"

syscall(geteuid,geteuid)

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
