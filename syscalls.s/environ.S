.section ".bss"
.align 8

.global environ
.type environ,object
environ:
.type __environ,object
.global __environ
__environ:
	.long 0
