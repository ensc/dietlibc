#include "start.h"

#ifdef __i386__
.text
.globl _start
_start:
#if 0
	popl	%ecx
	movl	%esp, %esi
	leal	4(%esp,%ecx,4),%eax
#else
	movl (%esp),%ecx	/* %ecx = argc */
	leal 4(%esp),%esi	/* %esi = argv */

	leal 4(%esi,%ecx,4),%eax	/* eax = 4*ecx+4 = envp */
#endif
	pushl %eax
	pushl %esi
	pushl %ecx
	movl %eax,environ

#ifdef WANT_DYNAMIC
/* in %edx we have the dynamic _fini ( register this if non null ) */
	test	%edx, %edx
	je	.Linit
	push	%edx
	call	atexit
	pop	%edx
.Linit:
	call	_init
	push	$_fini
	call	atexit
	pop	%edx
#endif

	call main
	pushl %eax
	call exit
.Lfefe:
	.size	 _start,.Lfefe-_start
#endif

