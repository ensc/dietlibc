#include "syscalls.h"

syscall(getxuid,getuid)

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
