.text
.align 0 
.global memcmp
.type	 memcmp,@function
memcmp:
	pushl %esi
	pushl %edi
	movl 12(%esp),%esi
	movl 16(%esp),%edi
	movl 20(%esp),%ecx
	
	rep cmpsb

	decl %esi
	decl %edi
	lodsb
	subb (%edi), %al
	movsx %al, %eax

	popl %edi
	popl %esi
	ret
