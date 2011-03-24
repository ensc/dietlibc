.text
.global __testandset
.type	__testandset,@function
__testandset:
	la	%r1,0(%r2)
	lhi	%r0,1
	lhi	%r2,0
	cs	%r2,%r0,0(%r1)
	xr	%r2,%r0
	br	%r14


#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
