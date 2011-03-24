#include "syscalls.h"

syscall(_newselect,select)

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
