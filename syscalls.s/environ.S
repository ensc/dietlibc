.section ".bss"
.align 8

.global environ
environ:
.global __environ
__environ:
	.long 0
