#include "syscalls.h"

#ifdef __NR_query_module
syscall(query_module,query_module)
#endif

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
