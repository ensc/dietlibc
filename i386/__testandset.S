.text
.global __testandset
__testandset:
	xorl	%eax, %eax
	movl	0x4(%esp),%edx
	incl	%eax
	xchgl	%eax,(%edx)
	ret
