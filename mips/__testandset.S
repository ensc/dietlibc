.text
.align 2
.global __testandsets
.ent	__testandsets
.type	__testandsets,@function
__testandset:
	.set	mips2
	ll	$2, 0($4)
	.set	noreorder
	bnez	$2, 1f
	li	$8, 1
	.set	reorder
	sc	$8, 0($4)
	beqz	$8, __testandset
1:	.set	mips0
	jr	$31
.end	__testandsets
