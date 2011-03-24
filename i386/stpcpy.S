.text
.global stpcpy
.type	stpcpy,@function
stpcpy:
	pushl %edi
	pushl %esi
	movl 0xc(%esp),  %edi
	movl 0x10(%esp), %esi
	cld
.Lcopy:
	lodsb
	stosb
	testb %al, %al
	jnz .Lcopy

	movl %edi, %eax
	decl %eax
	popl %esi
	popl %edi
	ret
.Lende:
.size stpcpy,.Lende-stpcpy

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
