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
	pushq	$_etext
	pushq	$.text
	call	monitor
	addq	$0x16, %rsp
#endif

#ifdef WANT_DYNAMIC
	call	_dyn_start
#else
	call	main
#endif

#ifdef PROFILING
	pushq	%rax
	call	_stop_monitor
	popq	%rdi
#else
	movq	%rax, %rdi	/* return value */
#endif
	call	exit
	hlt
.Lstart:
	.size	 _start,.Lstart-_start
#endif

