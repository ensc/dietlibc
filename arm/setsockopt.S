#ifdef __ARM_EABI__
	
#include "syscalls.h"

syscall(setsockopt,setsockopt)

#endif

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
