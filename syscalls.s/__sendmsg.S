#include "syscalls.h"

#ifdef __NR_sendmsg
syscall_weak(sendmsg,sendmsg,__libc_sendmsg)
#endif

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
