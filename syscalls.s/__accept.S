#include "syscalls.h"

#ifdef __NR_accept
syscall_weak(accept,accept,__libc_accept)
#endif

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
