#include "parisc/syscalls.h"

.text
.type umount,@function
.globl umount
.export umount
umount:
	ldi 0, %r25
	b __unified_syscall
	ldi __NR_umount2, %r20

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
