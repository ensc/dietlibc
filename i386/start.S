#include "start.h"
#include "dietfeatures.h"

#ifdef __i386__
.text
.globl _start
_start:
	movl	(%esp),%ecx		/* %ecx = argc */
	leal	4(%esp),%esi		/* %esi = argv */
	leal	4(%esi,%ecx,4),%eax	/* %eax = envp = (4*ecx)+%esi+4 */

#ifdef WANT_DYNAMIC
/* in %edx we have the ld.so dynamic _fini ( register this if non null ) */
	push	%edx
#endif

	pushl	%eax
	pushl	%esi
	pushl	%ecx
	movl	%eax,environ

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

