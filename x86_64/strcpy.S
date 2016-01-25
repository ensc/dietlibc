.text
.align 0
.global strcpy
#ifdef __PIE__
.hidden strcpy
#endif
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
.Lhere:
.size strcpy,.Lhere-strcpy
	.section	.note.GNU-stack,"",@progbits
