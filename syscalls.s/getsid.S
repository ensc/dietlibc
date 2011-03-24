#include "syscalls.h"

syscall(getsid,getsid)

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
