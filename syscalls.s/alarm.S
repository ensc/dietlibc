#include "syscalls.h"
#ifdef __NR_alarm
#ifdef __NR_alarm
syscall(alarm,alarm)
#endif
#endif

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
