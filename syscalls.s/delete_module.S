#include "syscalls.h"

#ifdef __NR_delete_module
syscall(delete_module,delete_module)
#endif

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
