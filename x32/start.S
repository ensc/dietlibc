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

#ifdef WANT_ELFINFO
	/* untested !! */

#ifdef __DYN_LIB
	movl	environ@GOTPCREL(%rip), %eax
#else
	leal	environ(%rip), %eax
#endif
	movl	%edx, (%eax)	/* environ */

1:	add	$4, %edx	/* incorment envp */
	cmpl	$0, -4(%edx)	/* load envp[-1] */
	jne	1b		/* ... until envp[-1]==NULL */

	movl	%edx, 4(%eax)	/* __elfinfo */
#else	/* WANT_ELFINFO */
#ifdef __DYN_LIB
	movl	environ@GOTPCREL(%rip), %eax
	movl	%edx, (%eax)
#else
	movl	%edx, environ(%rip)
#endif
#endif	/* WANT_ELFINFO */

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
	call	CALL_IN_STARTCODE
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

