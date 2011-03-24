#include "syscalls.h"

#ifdef __NR_ugetrlimit

syscall(ugetrlimit,getrlimit)

#elif defined(__NR_getrlimit)

syscall(getrlimit,getrlimit)

#endif

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
