#include "syscalls.h"

#ifdef __NR_setresgid32
syscall_weak(setresgid32,setresgid32,__libc_setresgid32)
#endif

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
