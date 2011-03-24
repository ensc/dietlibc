#include "syscalls.h"

syscall_weak(msync,msync,__libc_msync)

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
