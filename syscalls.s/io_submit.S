#include "syscalls.h"

syscall(io_submit,io_submit)

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
