.level 1.1
.text
.globl __testandset
__testandset:
	ldw 0(%sr0, %r26), %r28
	combf,= %r0, %r28, .Llocked
	ldi 1, %r25
	stw %r25, 0(%sr0, %r26)
.Llocked:
	bv,n %r0(%r2)
