#include "syscalls.h"

syscall(sethostname,sethostname)

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
