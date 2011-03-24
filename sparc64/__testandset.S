.text
.align 4
.global __testandset
__testandset:
	retl
	ldstub	[%o0], %o0

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
