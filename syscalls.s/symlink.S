#include "syscalls.h"

syscall(symlink,symlink)

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
