#include "dietwarning.h"

.text
.global __testandset
.type	__testandset,@function
__testandset:
	xorl	%eax,%eax
	incl	%eax
	xchg	%rax,(%rdi)
	ret

link_warning("__testandset", "warning: __testandset() was never tested!")

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
