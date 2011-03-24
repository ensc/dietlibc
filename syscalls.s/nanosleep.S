#include "syscalls.h"

syscall_weak(nanosleep,nanosleep,__libc_nanosleep)

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
