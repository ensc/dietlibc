#include "syscalls.h"

syscall_weak(write,write,__libc_write)

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
