#include "syscalls.h"

syscall(mq_unlink,mq_unlink)

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
