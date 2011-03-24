.text
.globl strcmp

strcmp:
	.PROC
	.CALLINFO
	.ENTRY
	ldb,ma 1(%r26), %r19
	ldb,ma 1(%r25), %r20
	cmpb,=,n %r19, %r0,  .Lexit
	cmpb,=,n %r19, %r20, strcmp
	nop
.Lexit:
	bv %r0(%rp)
	sub %r19, %r20, %ret0
	.EXIT
	.PROCEND
	.EXPORT strcmp

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
