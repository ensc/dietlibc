#include "syscalls.h"

#ifdef __NR_vmsplice
syscall(vmsplice,vmsplice)
#endif

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
