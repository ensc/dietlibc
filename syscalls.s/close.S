#include "syscalls.h"

syscall_weak(close,close,__libc_close)

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
