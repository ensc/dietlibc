#include "syscalls.h"

syscall_weak(syslog,__syscall_syslog,klogctl)

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
