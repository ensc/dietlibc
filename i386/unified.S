#include <dietfeatures.h>

.text
.global __unified_syscall
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
