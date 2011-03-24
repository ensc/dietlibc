#include "syscalls.h"

syscall(setfsgid,setfsgid)

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
