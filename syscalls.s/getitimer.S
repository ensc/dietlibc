#include "syscalls.h"

syscall(getitimer,getitimer)

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
