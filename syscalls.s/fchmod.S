#include "syscalls.h"

syscall(fchmod,fchmod)

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
