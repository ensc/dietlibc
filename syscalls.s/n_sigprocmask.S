#include "syscalls.h"

#ifdef __NR_sigprocmask
syscall_weak(sigprocmask,__old_sigprocmask,__n_sigprocmask)
#endif

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
