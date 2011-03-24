#include "syscalls.h"

#ifdef __NR_setuid32
syscall_weak(setuid32,setuid32,__libc_setuid32)
#endif

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
