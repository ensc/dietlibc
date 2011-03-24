#include "syscalls.h"

syscall(setsid,setsid)

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
