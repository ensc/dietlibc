#include "syscalls.h"

#ifdef __NR_sigpending
syscall_weak(sigpending,__old_sigpending,__n_sigpending)
#endif

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
