.text
.align 0 
.global strncmp
.type	 strncmp,@function
strncmp:
	pushl %esi
	pushl %edi

	movl 12(%esp),%esi
	movl 16(%esp),%edi
	movl 20(%esp),%ecx
	cld

.Lloop:
	jecxz .Lnotfound
	decl %ecx
	lodsb

	testb %al, %al
	jz .Ldone

	subb (%edi), %al
	jnz .Ldone
	incl %edi
	jmp .Lloop

.Lnotfound:
	xorb %al,%al
.Ldone:
	movsx %al, %eax
	popl %edi
	popl %esi
	ret
