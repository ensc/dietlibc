#include "dietfeatures.h"

#ifdef __x86_64__
.text
.global _start
_start:
	popq	%rdi			/* %rdi = argc */
	movq	%rsp,%rsi		/* %rsi = argv */
	pushq	%rdi
	
	leaq	8(%rsi,%rdi,8),%rdx	/* %rdx = envp = (8*rdi)+%rsi+8 */

	movq	%rdx, environ

#ifdef PROFILING
	pushl	$_etext
	pushl	$.text
	call	monitor
	addl	$0x8, %esp
#endif

	call	main

#ifdef PROFILING
	pushl	%eax
	call	_stop_monitor
	popl	%eax
#endif
	movq	%rax, %rdi	/* return value */
	call	exit
	hlt
.Lstart:
	.size	 _start,.Lstart-_start
#endif

