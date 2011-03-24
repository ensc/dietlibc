#include "syscalls.h"

#ifdef __NR_tee
syscall(tee,tee)
#endif

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
