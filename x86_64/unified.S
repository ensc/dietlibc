#include <dietfeatures.h>

#define SYS_exit 0x3c

.text

.weak exit
exit:
.global _exit
_exit:
.global _Exit
_Exit:
	mov	$SYS_exit,%al

.global __error_unified_syscall
.global __unified_syscall
.global __unified_syscall_16bit
#if defined(__ILP32__)
.global __unified_syscall_lp64
#endif
#ifdef __PIE__
.hidden exit
.hidden _exit
.hidden __error_unified_syscall
#if defined(__ILP32__)
.hidden __unified_syscall_lp64
#endif
.hidden __unified_syscall_16bit
.hidden __unified_syscall
#endif
__unified_syscall:
	mov	$0, %ah
__unified_syscall_16bit:
	movzwl	%ax, %eax
#if defined(__ILP32__)
	or	$0x40000000, %eax
__unified_syscall_lp64:
#endif
	mov	%rcx, %r10
	syscall
__error_unified_syscall:
	cmpq	$-132, %rax
	jbe	.Lnoerror
	negl	%eax
	pushq	%rax
	call	__errno_location
#ifdef IN_LDSO
	.hidden __errno_location
#endif
	popq	%rcx
	movl	%ecx,(%rax)
	orq	$-1, %rax
.Lnoerror:

/* here we go and "reuse" the return for weak-void functions */
#include "dietuglyweaks.h"

	ret
.Lhere:
	.size	 __unified_syscall,.Lhere-__unified_syscall
