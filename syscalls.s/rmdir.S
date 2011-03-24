#include "syscalls.h"

syscall(rmdir,rmdir)

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
