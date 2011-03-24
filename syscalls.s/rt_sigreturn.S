#include "syscalls.h"

syscall(rt_sigreturn,rt_sigreturn)

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
