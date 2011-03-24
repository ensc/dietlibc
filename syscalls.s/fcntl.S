#include "syscalls.h"

syscall_weak(fcntl,fcntl,__libc_fcntl)

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
