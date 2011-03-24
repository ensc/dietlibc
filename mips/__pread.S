#include "syscalls.h"

syscall(pread,__pread)

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
