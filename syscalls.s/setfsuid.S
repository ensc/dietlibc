#include "syscalls.h"

syscall(setfsuid,setfsuid)

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
