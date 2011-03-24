#include "syscalls.h"

syscall(pivot_root,pivot_root)

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
