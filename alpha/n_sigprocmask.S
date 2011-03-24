#include "syscalls.h"

syscall_weak(osf_sigprocmask,__old_sigprocmask,__n_sigprocmask)

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
