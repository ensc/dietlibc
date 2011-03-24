#include "syscalls.h"

syscall_weak(fsync,fsync,__libc_fsync)

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
