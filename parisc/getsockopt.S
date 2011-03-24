#include "syscalls.h"

syscall5(getsockopt, getsockopt);

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
