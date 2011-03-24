#include "syscalls.h"

#ifdef __NR_getresgid

syscall(getresgid,getresgid)

#endif

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
