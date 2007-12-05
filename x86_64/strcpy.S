.text
.align 0
.global strcpy
.type	 strcpy,@function
strcpy:
	mov %rdi,%rdx

.Lloop:
	lodsb
	stosb
	or %al, %al
	jnz .Lloop

	mov %rdx,%rax
	ret
