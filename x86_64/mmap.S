#include "syscalls.h"

.text
.global mmap
.type	mmap,@function
mmap:
	mov	$__NR_mmap,%al
	jmp	__unified_syscall
