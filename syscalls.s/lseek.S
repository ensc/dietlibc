#include "syscalls.h"

syscall_weak(lseek,lseek,__libc_lseek)

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
