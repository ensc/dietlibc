.text
.align 0 
.global memcmp
.type	 memcmp,@function
memcmp:
	xorl %eax,%eax
	orl 12(%esp),%eax
	jz .Lempty
	pushl %esi
	pushl %edi
	movl 12(%esp),%esi
	movl 16(%esp),%edi
	movl %eax,%ecx

	rep cmpsb

	decl %esi
	decl %edi
	lodsb
	subb (%edi), %al
	movsx %al, %eax

	popl %edi
	popl %esi
.Lempty:
	ret
