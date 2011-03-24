#include "syscalls.h"

#ifdef __NR_socketpair
syscall_weak(socketpair,socketpair,__libc_socketpair)
#endif

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
