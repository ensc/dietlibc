.text
.align 2
.global __testandsets
.ent	__testandsets
.type	__testandsets,@function
__testandset:
	ll	$2, 0($4)
	li	$8, 1
	sc	$8, 0($4)
	beqz	$8, __testandset
	jr	$31
.end	__testandsets
