#include "syscalls.h"

syscall(rt_sigprocmask,__rt_sigprocmask)

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
