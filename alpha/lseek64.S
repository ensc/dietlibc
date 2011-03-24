#include "syscalls.h"

.global lseek64
lseek64:
	br	lseek

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
