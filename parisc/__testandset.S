.level 1.1
.text
.globl __testandset
__testandset:
	ldcw 0(%sr0, %r26), %r28
	ldi 1,%r19
	bv %r0(%r2)
	xor %r28,%r19,%r28

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
