#include "syscalls.h"

syscall_weak(open,open,__libc_open)

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
