#include "syscalls.h"

syscall(io_setup,io_setup)

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
