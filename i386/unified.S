
#include <dietfeatures.h>

.text
#ifndef __DYN_LIB
.global __unified_syscall
#endif
__unified_syscall:
	movzbl	%al, %eax
	push	%edi
	push	%esi
	push	%ebx
	movl	%esp,%edi
	movl	0x10(%edi),%ebx
	movl	0x14(%edi),%ecx
	movl	0x18(%edi),%edx
	movl	0x1c(%edi),%esi
	movl	0x20(%edi),%edi
	int	$0x80
	cmp	$-124,%eax
	jbe	.Lnoerror
	neg	%eax
#ifdef WANT_THREAD_SAFE
	movl	%eax,%ebx
	call	__errno_location
	movl	%ebx,(%eax)
	xor	%eax,%eax
	dec	%eax
#else
	mov	%eax,errno
	sbb	%eax,%eax               # eax = eax - eax - CY = -1
#endif
.Lnoerror:
	pop	%ebx
	pop	%esi
	pop	%edi

/* here we go an "reused" the return for weak-void functions */
#include "dietuglyweaks.h"

	ret
