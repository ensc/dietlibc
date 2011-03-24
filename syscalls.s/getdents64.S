#include "dietfeatures.h"
#include "syscalls.h"

#ifdef __NR_getdents64
syscall(getdents64,getdents64)
#endif

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
