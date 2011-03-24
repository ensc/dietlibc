#include "syscalls.h"

#ifdef __NR_init_module
syscall(init_module,init_module)
#endif

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
