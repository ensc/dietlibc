#include "syscalls.h"

#ifndef __NR_fdatasync
#define __NR_fdatasync __NR_fsync
#endif
syscall_weak(fdatasync,fdatasync,__libc_fdatasync)

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
