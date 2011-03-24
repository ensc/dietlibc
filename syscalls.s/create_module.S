#include "syscalls.h"

#ifdef __NR_create_module
syscall(create_module,create_module)
#endif

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
