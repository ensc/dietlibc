#include "syscalls.h"

syscall(rt_sigtimedwait,__rt_sigtimedwait)

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
