#include "syscalls.h"

#ifdef __NR_set_thread_area
syscall(set_thread_area,set_thread_area)
#endif

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
