#include "syscalls.h"

#ifdef __NR_recvmsg
syscall_weak(recvmsg,recvmsg,__libc_recvmsg)
#endif

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
