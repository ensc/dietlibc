#include "syscalls.h"

syscall(set_tid_address,set_tid_address)

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
