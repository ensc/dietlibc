.global __testandset
.type __testandset,@function
.align 2
__testandset:
	lwarx	5,0,3
	add	0,4,5
	stwcx	0,0,3
	bne	__testandset
	blr
.size __testandset,.-__testandset

