#include "syscalls.h"

#ifdef __NR_recvfrom
syscall_weak(recvfrom,recvfrom,__libc_recvfrom)
#endif

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
