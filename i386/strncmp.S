.text
.align 0 
.global strncmp
.type	 strncmp,@function
strncmp:
	movl 4(%esp),%ebx
	movl 8(%esp),%edx
	movl 12(%esp),%ecx
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
	ret
