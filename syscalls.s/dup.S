#include "syscalls.h"

syscall(dup,dup)

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
