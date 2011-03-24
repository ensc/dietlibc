#include "syscalls.h"

#ifdef __NR_setfsgid32
syscall_weak(setfsgid32,setfsgid32,__libc_setfsgid32)
#endif

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
