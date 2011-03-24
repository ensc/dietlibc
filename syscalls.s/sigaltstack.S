#include "syscalls.h"

syscall_weak(sigaltstack,sigaltstack,__sigaltstack)

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
