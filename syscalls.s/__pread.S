#include "syscalls.h"

#ifdef __NR_pread64
syscall_weak(pread64,pread64,__libc_pread64)
#else
syscall_weak(pread,pread64,__libc_pread64)
#endif

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
