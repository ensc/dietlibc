.text
.align 4
.global __testandset
__testandset:
	mov	1, %o1
	ldstub	[%o0], %o1
	ret
	mov	%o1, %o0
