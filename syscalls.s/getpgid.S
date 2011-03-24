#include "syscalls.h"

syscall(getpgid,getpgid)

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
