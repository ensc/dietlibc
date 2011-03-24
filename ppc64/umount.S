#include "syscalls.h"

		.text
diet_proto(umount)
.umount:
		li 0, __NR_umount2
		li 4, 0
		b __unified_syscall

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
