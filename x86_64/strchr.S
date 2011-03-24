.text
.type   strchr,@function
.global strchr
.weak index
.type index,@function

index:
strchr:
	xchg	%rsi,%rdi
.Lloop:
	lodsb
	cmp	%al,%dil
	jz .Lfound
	or	%al,%al
	jnz .Lloop
	xor	%eax,%eax
	ret
.Lfound:
	lea	-1(%rsi),%rax
	ret

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
