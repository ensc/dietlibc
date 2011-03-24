#include "syscalls.h"

#ifdef __NR_setgid32
syscall_weak(setgid32,setgid32,__libc_setgid32)
#endif

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
