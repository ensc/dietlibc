#include "syscalls.h"

syscall(uname,uname)

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
