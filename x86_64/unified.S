#include <dietfeatures.h>

#define SYS_exit 0x3c

.text
.weak exit
exit:
.global _exit
_exit:
	mov	$SYS_exit,%al

#ifndef __DYN_LIB
.global __unified_syscall
#endif
__unified_syscall:
	movzbq	%al, %rax
	syscall
	cmp	$0, %eax
	jge	.Lnoerror
	negq	%rax
	pushq	%rax
	call	__errno_location
	popq	%rcx
	movl	%ecx,(%rax)
	xorq	%rax,%rax
	decq	%rax
.Lnoerror:

/* here we go and "reuse" the return for weak-void functions */
#include "dietuglyweaks.h"

	ret
