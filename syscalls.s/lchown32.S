#include "syscalls.h"

#ifdef __NR_lchown32
syscall_weak(lchown32,lchown32,__libc_lchown32)
#endif

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
