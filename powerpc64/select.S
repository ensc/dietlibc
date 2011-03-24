#include "syscalls.h"

syscall_weak(_newselect,select,__libc_select)

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
