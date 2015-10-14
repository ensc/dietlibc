#include "syscalls.h"

.text
.global mmap
.type	mmap,@function
mmap:
	/* stack: [ret a b c d e f] */
	movl 24(%esp),%eax
	test $4095,%eax
	jnz .Lerror
	shr $12,%eax
	movl %eax,24(%esp)
	jmp __mmap2
.Lerror:
	movl $-22,%eax	/* EINVAL */
	jmp __error_unified_syscall
#if 0
	mov	$__NR_mmap,%al
	lea	0x4(%esp,1),%edx
	push	%edx
	call	__unified_syscall
	pop	%ecx
	ret
#endif
.Lende3:
.size mmap,.Lende3-mmap
