#include "syscalls.h"

.text
.global mmap
.type	mmap,@function
mmap:
	mov	%rcx,%r10
	mov	$__NR_mmap,%rax
	jmp	__unified_syscall
