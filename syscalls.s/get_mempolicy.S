#include "syscalls.h"

#ifdef __NR_get_mempolicy
syscall(get_mempolicy,get_mempolicy)
#endif

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
