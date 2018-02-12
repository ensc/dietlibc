#include "dietfeatures.h"

.text
.global _start
_start:
#ifdef WANT_DYNAMIC
	movq	%rdx, %rcx		/* %rcx = dynamic fini */
#endif
	popq	%rdi			/* %rdi = argc */
	movq	%rsp,%rsi		/* %rsi = argv */
	pushq	%rdi
	
	leaq	8(%rsi,%rdi,8),%rdx	/* %rdx = envp = (8*rdi)+%rsi+8 */

#ifndef IN_LDSO
#ifdef WANT_ELFINFO
#ifdef __PIC__
	movq	environ@GOTPCREL(%rip), %rax
#else
	leaq	environ(%rip), %rax
#endif
	movq	%rdx, %rbx
	movq	%rdx, (%rax)	/* environ */

1:	add	$8, %rbx	/* increment envp */
	cmpq	$0, -8(%rbx)	/* load envp[-1] */
	jne	1b		/* ... until envp[-1]==NULL */

	movq	%rbx, 8(%rax)	/* __elfinfo */
#else
#ifdef __PIC__
	movq	environ@GOTPCREL(%rip), %rax
	movq	%rdx, (%rax)
#else
	movq	%rdx, environ(%rip)
	.hidden environ
#endif
#endif

#ifdef PROFILING
	pushq	%rdi			/* save reg args */
	pushq	%rsi
	pushq	%rdx
	pushq	%rcx

	leaq	_etext(%rip), %rsi	/* highpc */
	leaq	 .text(%rip), %rdi	/* lowpc */
	call	monitor

	popq	%rcx			/* restore reg args */
	popq	%rdx
	popq	%rsi
	popq	%rdi
#endif

#ifdef WANT_DYNAMIC
	call	_dyn_start
	.hidden _dyn_start
#else
#ifdef __PIE__
	call	CALL_IN_STARTCODE_PIE
	.hidden CALL_IN_STARTCODE_PIE
#else
	call	CALL_IN_STARTCODE
	.hidden CALL_IN_STARTCODE
#endif
#endif

#ifdef PROFILING
	pushq	%rax
	call	_stop_monitor
	popq	%rdi
#else
	movq	%rax, %rdi	/* return value */
#endif

#else
	call	main
	.hidden main
#endif

#if defined(__ILP32__)
	mov	$SYS_exit,%eax
	syscall
#else
	call	exit
#endif
	hlt
.Lstart:
	.size	 _start,.Lstart-_start
	.section	.note.GNU-stack,"",@progbits
