.text
.align 0 
.global strncmp
.type	 strncmp,@function
strncmp:
	push %ebx
	movl 8(%esp),%ebx
	movl 12(%esp),%edx
	movl 16(%esp),%ecx
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
	xorb %eax,%eax
.Lnotequal:
	movsx %al, %eax
	popl %ebx
	ret
