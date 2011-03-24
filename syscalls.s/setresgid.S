#include "syscalls.h"

#ifdef __NR_setresgid

syscall(setresgid,setresgid)

#endif

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
