.text
.align 0 
.global strcmp
.type	 strcmp,@function
strcmp:
	movl 4(%esp),%ecx
	movl 8(%esp),%edx
.Lloop:
	movb (%ecx),%al
	incl %edx
	incl %ebx
	movb %al,%ah
	subb -1(%edx),%al
	jnz .Lnotequal
	testb %ah,%ah
	jnz .Lloop

	xorb %eax,%eax
.Lnotequal:
	movsx %al, %eax
	ret
