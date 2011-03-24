#include "syscalls.h"

#ifdef __NR_fallocate
syscall(fallocate,fallocate)
#endif

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
