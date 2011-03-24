#include "dietfeatures.h"
#include "syscalls.h"

#ifdef __NR_ftruncate64
#ifdef WANT_LARGEFILE_BACKCOMPAT
syscall(ftruncate64,__dietlibc_ftruncate64)
#else
syscall(ftruncate64,ftruncate64)
#endif
#endif

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
