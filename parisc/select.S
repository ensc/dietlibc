#include "syscalls.h"

syscall5(select, select);

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
