.section ".bss"
.align 8

.weak errno
errno:
.global __errno
__errno:
	.long 0

.weak environ
environ:
.global __environ
__environ:
	.long 0
