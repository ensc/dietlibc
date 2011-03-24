#include "syscalls.h"

#ifdef __NR_pause
syscall_weak(pause,pause,__libc_pause)
#endif

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
