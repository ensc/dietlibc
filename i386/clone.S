#include <asm/unistd.h>

.text
.weak clone
clone:
.global __clone
__clone:
	movl	4(%esp), %ecx	/* have non null thread_funcion */
	testl	%ecx, %ecx
	je	.Lclone_error

	movl	8(%esp), %ecx	/* have non null child_stack pointer */
	testl	%ecx, %ecx
	je	.Lclone_error

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
	je	.Lstart_thread
	ret

.Lstart_thread:
	xorl	%ebp,%ebp
	call	*%ebx
	pushl	%eax
	call	_exit

.Lclone_error:
	negl	%eax
	pushl	%eax
	call	__errno_location
	popl	%ecx
	movl	%ecx, (%eax)
	xorl	%eax, %eax
	decl	%eax
	ret
