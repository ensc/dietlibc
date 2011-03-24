#include "syscalls.h"

#ifdef __NR_recv
syscall_weak(recv,recv,__libc_recv)
#endif

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
