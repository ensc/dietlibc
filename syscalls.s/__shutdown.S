#include "syscalls.h"

#ifdef __NR_shutdown
syscall_weak(shutdown,shutdown,__libc_shutdown)
#endif

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
