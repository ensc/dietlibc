#include "syscalls.h"

#ifdef __NR_ipc
syscall(ipc,__ipc)
#endif

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
