#include "dietfeatures.h"
#include "syscalls.h"

#ifdef __NR_truncate64
#ifdef WANT_LARGEFILE_BACKCOMPAT
syscall(truncate64,__dietlibc_truncate64)
#else
syscall(truncate64,truncate64)
#endif
#endif

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
