#include "syscalls.h"

#ifdef __NR_vserver
syscall(vserver,vserver)
#endif

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
