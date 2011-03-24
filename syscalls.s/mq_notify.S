#include "syscalls.h"

syscall(mq_notify,mq_notify)

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
