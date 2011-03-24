#include "syscalls.h"

#ifdef __NR_setregid32
syscall_weak(setregid32,setregid32,__libc_setregid32)
#endif

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
