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
	cmpq	$0, %rax
	jge	.Lnoerror
	negq	%rax
	movl	%eax, %ecx
	call	__errno_location
	movl	%ecx, (%rax)
	mov	$-1, %rax
.Lnoerror:

/* here we go and "reuse" the return for weak-void functions */
#include "dietuglyweaks.h"

	ret
