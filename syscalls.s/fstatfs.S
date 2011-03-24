#include "syscalls.h"

syscall(fstatfs,fstatfs)

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
