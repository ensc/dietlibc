#include "syscalls.h"

syscall(io_destroy,io_destroy)

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
