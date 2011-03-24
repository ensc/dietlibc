#include "syscalls.h"

syscall(execve,execve)

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
