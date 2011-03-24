#include "syscalls.h"

#ifdef __NR_sigsuspend
syscall_weak(sigsuspend,__old_sigsuspend,__n_sigsuspend)
#endif

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
