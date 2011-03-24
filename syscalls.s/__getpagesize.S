#include "syscalls.h"

#ifdef __NR_getpagesize
syscall(getpagesize,getpagesize)
#endif

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
