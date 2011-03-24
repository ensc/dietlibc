#include "syscalls.h"

syscall(ptrace,__diet_ptrace)

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
