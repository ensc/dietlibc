#include "syscalls.h"

#ifdef __NR_fadvise64
syscall(fadvise64,fadvise64)

#ifndef __NR_fadvise64_64
.set   posix_fadvise, fadvise64
.globl posix_fadvise
#endif

#endif

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
