#include "syscalls.h"

#ifdef __NR_get_thread_area
syscall(get_thread_area,get_thread_area)
#endif

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
