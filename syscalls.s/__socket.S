#include "syscalls.h"

#ifdef __NR_socket
syscall_weak(socket,socket,__libc_socket)
#endif

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
