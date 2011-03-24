#include "syscalls.h"

#ifdef __NR_arm_fadvise64_64
syscall(arm_fadvise64_64, __arm_fadvise64_64);
#endif

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
