#include "dietfeatures.h"

#ifdef __i386__
.text
.global _start
_start:
#if 1
	popl	%ecx			/* %ecx = argc */
	movl	%esp,%esi		/* %esi = argv */
	pushl	%ecx
#else
	movl	(%esp),%ecx		/* %ecx = argc */
	leal	4(%esp),%esi		/* %esi = argv */
#endif
	leal	4(%esi,%ecx,4),%eax	/* %eax = envp = (4*ecx)+%esi+4 */

#ifdef WANT_DYNAMIC
/* in %edx we have the ld.so dynamic _fini ( register this if non null ) */
	push	%edx
#endif

	pushl	%eax
	pushl	%esi
	pushl	%ecx
#ifdef __DYN_LIB
	call	1f
1:	popl	%ebx
	addl	$_GLOBAL_OFFSET_TABLE_+[.-1b],%ebx
	movl	environ@GOT(%ebx),%ecx
	movl	%eax,(%ecx)
#else
	movl	%eax,environ
#endif

#ifdef WANT_DYNAMIC
	call	_dyn_start
#else
	call	main
#endif

	pushl	%eax
	call	exit
	hlt	/* die now ! will ya ... */
.Lstart:
	.size	 _start,.Lstart-_start
#endif

