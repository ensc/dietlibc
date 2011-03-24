#include "syscalls.h"

#ifdef __NR_utime
syscall(utime,utime)
#endif

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
