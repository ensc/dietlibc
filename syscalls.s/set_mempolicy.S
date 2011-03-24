#include "syscalls.h"

#ifdef __NR_set_mempolicy
syscall(set_mempolicy,set_mempolicy)
#endif

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
