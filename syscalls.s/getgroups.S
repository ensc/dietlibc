#include "syscalls.h"

syscall(getgroups,getgroups)

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
