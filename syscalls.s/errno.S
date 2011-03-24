.section ".bss"
.align 8

.type errno,object
.weak errno
errno:
	.long 0
.size errno,.-errno

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
