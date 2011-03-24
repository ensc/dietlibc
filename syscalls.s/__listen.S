#include "syscalls.h"

#ifdef __NR_listen
syscall_weak(listen,listen,__libc_listen)
#endif

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
