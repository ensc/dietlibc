#include "syscalls.h"

.text
.global mmap
.type	mmap,@function
mmap:
	mov	%rcx,%r10
	mov	$__NR_mmap,%rax
	call	__unified_syscall
	ret
