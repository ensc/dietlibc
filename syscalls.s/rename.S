#include "syscalls.h"

syscall(rename,rename)

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
