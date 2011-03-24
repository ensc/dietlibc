#include "syscalls.h"

#ifdef __NR_signalfd
syscall(signalfd,__signalfd)
#endif

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
