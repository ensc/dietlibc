.text
.align 0 
.global strcmp
.type	 strcmp,@function
strcmp:
	pushl %esi
	pushl %edi

	movl 12(%esp),%esi
	movl 16(%esp),%edi
	decl %edi
	cld

.Lloop:
	incl %edi
	lodsb

	testb %al, %al
	jz .Ldone

	subb (%edi), %al
	jz .Lloop

.Ldone:
	movsx %al, %eax
	popl %edi
	popl %esi
	ret
