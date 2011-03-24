#include "syscalls.h"

#ifdef __NR_setreuid32
syscall_weak(setreuid32,setreuid32,__libc_setreuid32)
#endif

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
