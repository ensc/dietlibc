#include "syscalls.h"

syscall(reboot,__reboot)

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
