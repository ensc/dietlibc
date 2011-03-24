#include "syscalls.h"

syscall(add_key,add_key)

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
