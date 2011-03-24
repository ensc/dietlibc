#include "syscalls.h"

#ifdef __NR_statfs64
syscall(statfs64,__dietlibc_statfs64)
#endif

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
