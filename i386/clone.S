#include "syscalls.h"

.text
.type clone,@function
.weak clone
clone:
.type __clone,@function
.global __clone
__clone:
	movl	4(%esp), %ecx	/* have non null thread_funcion */
	jecxz	.Lclone_error

	movl	8(%esp), %ecx	/* have non null child_stack pointer */
	jecxz	.Lclone_error

	/* put the parameter on thread stack */
	subl	$8, %ecx

	movl	16(%esp), %eax	/* arg */
	movl	%eax, 4(%ecx)

	movl	4(%esp), %eax	/* thread_func */
	movl	%eax, 0(%ecx)

	/* the syscall */
	pushl	%ebx
	movl	16(%esp), %ebx	/* flags */
	movl	$__NR_clone, %eax
	int	$0x80
	popl	%ebx

	testl	%eax, %eax
	jl	.Lclone_error
	jne	.Lreturn
.Lstart_thread:
	xorl	%ebp,%ebp
	call	*%ebx
	pushl	%eax
	call	_exit

.Lclone_error:
	negl	%eax
	pushl	%eax
#ifdef PIC
	call	1f
1:	popl	%ebx
	addl	$_GLOBAL_OFFSET_TABLE_+[.-1b], %ebx
	call	__errno_location@PLT
#else
	call	__errno_location
#endif
	popl	%ecx
	movl	%ecx, (%eax)
	xorl	%eax, %eax
	decl	%eax
.Lreturn:	
	ret
