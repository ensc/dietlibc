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


#ifdef WANT_ELFINFO
#ifdef __DYN_LIB
	movq	environ@GOTPCREL(%rip), %rax
#else
	leaq	environ(%rip), %rax
#endif
	movq	%rdx, (%rax)	/* environ */

1:	add	$8, %rdx	/* increment envp */
	cmpq	$0, -8(%rdx)	/* load envp[-1] */
	jne	1b		/* ... until envp[-1]==NULL */

	movq	%rdx, 8(%rax)	/* __elfinfo */
#else
#ifdef __DYN_LIB
	movq	environ@GOTPCREL(%rip), %rax
	movq	%rdx, (%rax)
#else
	movq	%rdx, environ(%rip)
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
#else
	call	CALL_IN_STARTCODE
#endif

#ifdef PROFILING
	pushq	%rax
	call	_stop_monitor
	popq	%rdi
#else
	movq	%rax, %rdi	/* return value */
#endif
#if defined(__ILP32__)
	xor %rax,%rax		/* do exit inline with x64 syscall number so it also works if the kernel does not have x32 support */
	inc %rax
	syscall
#else
	call	exit
#endif
	hlt
.Lstart:
	.size	 _start,.Lstart-_start

