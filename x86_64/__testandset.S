#include "dietwarning.h"

.text
.global __testandset
#ifdef __PIE__
.hidden __testandset
#endif
.type	__testandset,@function
__testandset:
	xorl	%eax,%eax
	incl	%eax
	xchg	%eax,(%rdi)
	ret
