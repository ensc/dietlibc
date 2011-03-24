#include "syscalls.h"

syscall(setxattr,setxattr)

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
