.text
.align 0
.global strcpy
.type	 strcpy,@function
strcpy:
	mov %rdi,%rdx

.Lloop:
	lodsb
	test %al, %al
	stosb
	jnz .Lloop

	mov %rdx,%rax
	ret

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
