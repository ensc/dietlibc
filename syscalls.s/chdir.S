#include "syscalls.h"

syscall(chdir,chdir)

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
