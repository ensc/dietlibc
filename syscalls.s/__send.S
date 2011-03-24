#include "syscalls.h"

#ifdef __NR_send
syscall_weak(send,send,__libc_send)
#endif

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
