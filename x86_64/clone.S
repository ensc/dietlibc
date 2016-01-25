#include "syscalls.h"
#include <errno.h>
#include "dietwarning.h"

.text
.type clone,@function
.weak clone
clone:
.type __clone,@function
.global __clone
#ifdef __PIE__
.hidden __clone
.hidden clone
#endif
__clone:
	movl	$-EINVAL,%eax
	testq	%rdi,%rdi	/* have non null thread_funcion */
	jz	.Lclone_error
	testq	%rsi,%rsi	/* have non null child_stack pointer */
	jz	.Lclone_error

	/* put the parameters on thread stack */
	subq	$16,%rsi
	movq	%rcx,8(%rsi)	/* thread parameter */
	movq	%rdi,0(%rsi)	/* thread function */

	/* args to this function:		args to syscall
	   ----------------------------------------------------
	   rdi = function to be called		rdi = flags
	   rsi = child_stack			rsi = child_stack
	   rdx = flags				rdx = ptid
	   rcx = arg				r10 = ctid
	   r8 = ptid				r8 = regs
	   r9 = tls
	   stack = ctid */

	movq	%rdx,%rdi
	/* if CLONE_PARENT_SETTID is set, r8 goes into rdx */
	test $0x100000,%rdi
	jz .Lnocps
	movq	%r8,%rdx
	/* if CLONE_SETTLS is set, r9 goes into r8 */
	test $0x80000,%rdi
	jz .Lcps_notls
	movq	%r9,%r8
	/* if CLONE_CHILD_SETTID is set, stack goes into r10 */
	test $0x1000000,%rdi
	jz .Ldone
	movq	8(%rsp),%r10
	jmp .Ldone

.Lnocps:
	/* if CLONE_SETTLS is set, r8 goes into r8 */
	test $0x80000,%rdi
	jz .Lnocps_notls
/*	movq	%r8,%r9 */
.Lcps_notls:
	/* if CLONE_CHILD_SETTID is set, r9 goes into r10 */
	test $0x1000000,%rdi
	jz .Ldone
	movq	%r9,%r10
	jmp .Ldone

.Lnocps_notls:
	/* if CLONE_CHILD_SETTID is set, r8 goes into r10 */
	test $0x1000000,%rdi
	jz .Ldone
	movq	%r8,%r10

.Ldone:

	/* syscall */
	movl	$__NR_clone,%eax
	syscall
	cmpl	$0,%eax
	jl	.Lclone_error
	jne	.Lreturn

.thread_start:
	/* call thread function */
	popq	%rax
	popq	%rdi
	call	*%rax
	/* exit a thread */
	movq	%rax,%rdi
#ifdef __PIC__
	jmp	thrd_exit@PLT
#else
	jmp	thrd_exit
#endif

.Lclone_error:
	/* set errno */
	negl	%eax
	pushq	%rax
#ifdef __PIC__
	call	__errno_location@PLT
#else
	call	__errno_location
#endif
	popq	%rcx
	movl	%ecx,(%rax)
	orq	$-1, %rax

.Lreturn:
	/* just return */
	ret
.Lhere:
.size clone,.Lhere-clone

	.section	.note.GNU-stack,"",@progbits
