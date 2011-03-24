#include "syscalls.h"

#ifdef __NR_mmap2
syscall6(mmap2,__mmap2)
#endif


#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
