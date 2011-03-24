#include "syscalls.h"

syscall(msgget,msgget)

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
