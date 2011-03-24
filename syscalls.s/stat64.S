#include "dietfeatures.h"
#include "syscalls.h"

#ifdef __NR_stat64
#ifdef WANT_LARGEFILE_BACKCOMPAT
syscall(stat64,__dietlibc_stat64)
#else
syscall(stat64,stat64)
#endif
#endif

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
