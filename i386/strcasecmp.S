.text
.globl strcasecmp

strcasecmp:
	pushl %esi
	movl 0x8(%esp), %esi
	movl 0xc(%esp), %edx
	xorl %eax, %eax
	xorl %ecx, %ecx
.Lloop:
	lodsb
	movb (%edx), %cl
	incl %edx
	or %al, %al
	jz .Lret
	cmp $'A', %al
	jnge .Lcmp
	cmp $'z', %al
	jg .Lcmp
	or $0x20, %al
	or $0x20, %cl
.Lcmp:
	subl %ecx, %eax
	jz .Lloop
.Lret:
	popl %esi
	ret
