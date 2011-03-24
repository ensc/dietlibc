#include "syscalls.h"

#ifdef __NR_ioperm
syscall(ioperm,ioperm)
#endif

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
