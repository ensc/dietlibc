.section ".bss"
.align 8

.global environ
environ:
.type __environ,object
.global __environ
__environ:
	.long 0
