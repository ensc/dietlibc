#include "syscalls.h"

syscall_weak(fork,fork,__libc_fork)

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
