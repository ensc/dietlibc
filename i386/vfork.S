.text
.globl vfork
vfork:
	popl %edx
	xorl %eax,%eax
	movb $190, %al
	int $0x80
	jmpl *%edx
.size vfork, . - vfork
