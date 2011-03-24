#include "syscalls.h"

syscall(getpid,getpid)

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
