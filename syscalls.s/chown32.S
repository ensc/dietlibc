#include "syscalls.h"

#ifdef __NR_chown32
syscall_weak(chown32,chown32,__libc_chown32)
#endif

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
