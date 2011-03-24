#include "syscalls.h"

#ifdef __NR_waitpid
syscall_weak(waitpid,waitpid,__libc_waitpid)
#endif

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
