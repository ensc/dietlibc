#include "syscalls.h"

#ifdef __NR_mmap

syscall(mmap,mmap)

#endif

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
