#include "syscalls.h"

syscall_weak(read,read,__libc_read)

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
