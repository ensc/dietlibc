#include "syscalls.h"

#ifdef __NR_geteuid32
syscall_weak(geteuid32,geteuid32,__libc_geteuid32)
#endif

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
