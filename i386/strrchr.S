.text
.globl strrchr
strrchr:
	movl 0x4(%esp), %edx
	movb 0x8(%esp), %cl
	xorl %eax, %eax
	decl %edx
.Lloop:
	incl %edx
	cmpb $0x0,(%edx)
	jz .Lret
	cmpb %cl, (%edx)
	jnz .Lloop
	movl %edx, %eax
	jmp .Lloop
.Lret:
	ret
