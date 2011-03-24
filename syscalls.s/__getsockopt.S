#include "syscalls.h"

#ifdef __NR_getsockopt
syscall_weak(getsockopt,getsockopt,__libc_getsockopt)
#endif

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
