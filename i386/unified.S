#include <dietfeatures.h>

.text
.global __unified_syscall
__unified_syscall:
	and	$0xff,%eax
	push	%edi
	push	%esi
	push	%ebx
	movl	0x10(%esp),%ebx
	movl	0x14(%esp),%ecx
	movl	0x18(%esp),%edx
	movl	0x1c(%esp),%esi
	movl	0x20(%esp),%edi
	int	$0x80
	cmp	$-124,%eax
	jbe	.Lnoerror
#ifdef WANT_THREAD_SAVE
	movl	%eax,%ebx
	call	__errno_location
	neg	%ebx
	movl	%ebx,(%eax)
#else
	neg	%eax
	mov	%eax,errno
#endif
	xor	%eax,%eax
	dec	%eax
.Lnoerror:
	pop	%ebx
	pop	%esi
	pop	%edi
	ret
