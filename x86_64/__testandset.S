#include "dietwarning.h"

.text
.global __testandset
.type	__testandset,@function
__testandset:
	xor	%rax, %rax
	mov	0x8(%rsp),%rdx
	inc	%rax
	xchg	%rax,(%rdx)
	ret

link_warning("__testandset", "warning: __testandset() was never tested!")
