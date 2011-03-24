#include "syscalls.h"

syscall(utimes,utimes)

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
