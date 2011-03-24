#include "syscalls.h"

#ifdef __NR_getpeername
syscall_weak(getpeername,getpeername,__libc_getpeername)
#endif

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
