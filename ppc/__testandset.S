.global __testandset
.type __testandset,@function
.align 2
__testandset:
	lwarx	5,0,3
	li	0,1
	stwcx.	0,0,3
	bne	__testandset
	mr	3,5
	blr
.size __testandset,.-__testandset

