.text
.align 0 
.global strcat
.type	 strcat,@function
strcat:
	pushl %edi
	pushl %esi

	movl 12(%esp), %edi
	movl 16(%esp), %esi

	pushl %edi

	xorl %eax, %eax
	xorl %ecx, %ecx
	decl %ecx
	repne scasb
	decl %edi
	
.loop:
	lodsb
	stosb
	testb %al, %al
	jnz .loop
	
	popl %eax
	popl %esi
	popl %edi
	ret
