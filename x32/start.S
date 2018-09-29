#include "dietfeatures.h"

.text
.global _start
_start:
#ifdef WANT_DYNAMIC
	movq	%rdx, %rcx		/* %rcx = dynamic fini */
#endif
	movl	(%esp), %edi		/* %edi = argc */
	lea	4(%esp), %esi		/* %esi = argv */
	
	leaq	4(%rsi,%rdi,4),%rdx	/* %edx = envp = (4*edi)+%esi+4 */

#ifdef __DYN_LIB
	movl	environ@GOTPCREL(%rip), %eax
	movl	%edx, (%eax)
#else
	movl	%edx, environ(%rip)
#endif

#ifdef PROFILING
	pushl	%edi			/* save reg args */
	pushl	%esi
	pushl	%edx
	pushl	%ecx

	leal	_etext(%eip), %esi	/* highpc */
	leal	 .text(%eip), %edi	/* lowpc */
	call	monitor

	popl	%ecx			/* restore reg args */
	popl	%edx
	popl	%esi
	popl	%edi
#endif

#ifdef WANT_DYNAMIC
	call	_dyn_start
#else
#ifdef __PIE__
	call	CALL_IN_STARTCODE_PIE
#else
	call	CALL_IN_STARTCODE
#endif
#endif

#ifdef PROFILING
	pushl	%eax
	call	_stop_monitor
	popl	%edi
#else
	movl	%eax, %edi	/* return value */
#endif
	call	exit
	hlt
.Lstart:
	.size	 _start,.Lstart-_start

