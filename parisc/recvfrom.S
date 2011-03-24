#include "syscalls.h"

syscall6(recvfrom, recvfrom);

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
