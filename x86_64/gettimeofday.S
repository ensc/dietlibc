.text
.global gettimeofday
.type gettimeofday,@function
gettimeofday:
	mov	$0xffffffffff600000,%rax
	callq	*%rax
	jmp __error_unified_syscall
.Lhere:
	.size	 gettimeofday,.Lhere-gettimeofday

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
