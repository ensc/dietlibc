#include "syscalls.h"

#ifdef __NR_getuid32
syscall_weak(getuid32,getuid32,__libc_getuid32)
#endif

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
