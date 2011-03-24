#include "syscalls.h"

#ifdef __NR_time
syscall(time,time)
#endif

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
