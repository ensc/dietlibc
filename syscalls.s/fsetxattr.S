#include "syscalls.h"

syscall(fsetxattr,fsetxattr)

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
