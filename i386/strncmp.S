.text
.align 0 
.global strncmp
.type	 strncmp,@function
strncmp:
	push %ebx
	movl %esp,%ebx
	movl 12(%ebx),%edx
	movl 16(%ebx),%ecx
	movl 8(%ebx),%ebx
.Lloop:
	jecxz .Ldone
	decl %ecx
	movb (%ebx),%al
	incl %edx
	incl %ebx
	movb %al,%ah
	subb -1(%edx),%al
	jnz .Lnotequal
	testb %ah,%ah
	jnz .Lloop

.Ldone:
	xorl %eax,%eax
.Lnotequal:
	movsx %al, %eax
	popl %ebx
	ret
