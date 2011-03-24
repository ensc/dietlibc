#include "syscalls.h"

syscall(getcwd,__syscall_getcwd)

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
