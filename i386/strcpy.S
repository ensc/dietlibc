
.text
.align 0 
.global strcpy
.type	 strcpy,@function
strcpy:
	pushl %esi
	pushl %edi
	
	movl 12(%esp), %ebx
	movl 16(%esp), %esi
	movl %ebx, %edi
	cld

.Lloop:
	lodsb
	stosb
	orb %al, %al
	jnz .Lloop

	popl %edi
	popl %esi
	movl %ebx,%eax
	ret
