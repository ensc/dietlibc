#include "syscalls.h"

#ifdef __NR_sysfs
syscall(sysfs,sysfs)
#endif

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
