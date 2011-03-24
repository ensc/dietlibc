#include "dietfeatures.h"
#include "syscalls.h"

#ifdef __NR_fcntl64
#ifdef WANT_LARGEFILE_BACKCOMPAT
syscall(fcntl64,__dietlibc_fcntl64)
#else
syscall(fcntl64,fcntl64)
#endif
#endif

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
