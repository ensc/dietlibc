.text
.align 0 
.global memcpy
.type	 memcpy,@function
memcpy:
	pushl %esi
	pushl %edi
	movl 12(%esp),%esi
	movl 16(%esp),%edi
	movl 20(%esp),%ecx
	movl %edi, %eax
	rep movsb
	popl %edi
	popl %esi
	ret
