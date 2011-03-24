
.text
.type   strchr,@function
.global strchr
.weak index
.type index,@function

index:
strchr:
	movl	4(%esp),%ecx
	movb	8(%esp),%dl
.Lloop:
	movb	(%ecx),%al
	cmpb	%al,%dl
	jz .Lfound
	incl	%ecx
	testb	%al,%al
	jnz .Lloop
	xorl	%ecx,%ecx
.Lfound:
	movl	%ecx,%eax
	ret

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
