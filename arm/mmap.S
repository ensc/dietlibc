#include <errno.h>
#include "syscalls.h"

.text
.global mmap
mmap:
	stmdb	sp!, {r0, r1, r2, r3}
	mov	r0, sp
	swi	__NR_mmap
	add	sp, sp, #16
	b	__unified_syscall
