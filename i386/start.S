#include "start.h"
#include "dietfeatures.h"

#ifdef __i386__
.text
.globl _start
_start:
	movl	(%esp),%ecx		/* %ecx = argc */
	leal	4(%esp),%esi		/* %esi = argv */
	leal	4(%esi,%ecx,4),%eax	/* %eax = (4*ecx)+%esi+4 = envp */

	pushl	%eax
	pushl	%esi
	pushl	%ecx
	movl	%eax,environ

#ifdef WANT_DYNAMIC
/* in %edx we have the dynamic _fini ( register this if non null ) */
	push	%edx
	call	_dyn_start
#else
	call	main
#endif
	pushl	%eax
	call	exit
.Lfefe:
	.size	 _start,.Lfefe-_start
#endif

