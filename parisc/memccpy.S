.text
.globl memccpy

memccpy:
	.PROC
	.CALLINFO
	.ENTRY
	addi,> -1, %r23, %r23
	b,n .Lret
	ldb,ma 1(%r25), %r19
	cmpb,<> %r19, %r24, memccpy
	stb,ma %r19, 1(%r26)
	bv %r0(%rp)
	copy %r26, %ret0
.Lret:
	bv %r0(%rp)
	ldi 0, %ret0
	.EXIT
	.PROCEND
	.EXPORT memccpy
