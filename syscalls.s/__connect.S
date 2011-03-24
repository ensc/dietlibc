#include "syscalls.h"

#ifdef __NR_connect
syscall_weak(connect,connect,__libc_connect)
#endif

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
