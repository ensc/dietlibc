.section ".bss"
.align 8

.type environ,object
.weak environ
.type __environ,object
.weak __environ
__environ:
environ:
	.long 0

