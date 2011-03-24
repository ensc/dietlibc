#include "syscalls.h"

#ifdef __NR_sendto
syscall_weak(sendto,sendto,__libc_sendto)
#endif

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
