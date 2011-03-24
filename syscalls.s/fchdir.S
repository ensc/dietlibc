#include "syscalls.h"

syscall(fchdir,fchdir)

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
