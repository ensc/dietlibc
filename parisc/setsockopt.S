#include "syscalls.h"

syscall5(setsockopt, setsockopt);

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
