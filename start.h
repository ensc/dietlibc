.section ".bss"
	.align 8

	.weak errno
errno:
	.long 0

	.weak environ
environ:
	.long 0
