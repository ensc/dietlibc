#include "syscalls.h"

.text
.global mmap
.type	mmap,@function
mmap:
	mov	$__NR_mmap,%rax
	call	__unified_syscall
	ret
