.section ".bss"
.align 8

.type errno,object
.weak errno
errno:
//.global __errno
//__errno:
	.long 0
