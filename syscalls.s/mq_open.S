#include "syscalls.h"

syscall(mq_open,mq_open)

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
