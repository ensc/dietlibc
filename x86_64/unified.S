#include <dietfeatures.h>

#define SYS_exit 0x3c

.text
.weak exit
exit:
.global _exit
_exit:
	movq	$SYS_exit,%rax

#ifndef __DYN_LIB
.global __unified_syscall
#endif
__unified_syscall:
	syscall
	cmpq	$0, %rax
	jge	.Lnoerror
	negq	%rax
	movl	%eax, %ebx
	call	__errno_location
	movl	%ebx, (%rax)
	mov	$-1, %rax
.Lnoerror:

/* here we go and "reuse" the return for weak-void functions */
#include "dietuglyweaks.h"

	ret
