#include "syscalls.h"

#ifdef __NR_bind
syscall_weak(bind,bind,__libc_bind)
#endif

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
