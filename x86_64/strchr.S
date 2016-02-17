.text
.type   strchr,@function
.global strchr
.weak index
#ifdef __PIE__
.hidden strchr
.hidden index
#endif
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
.Lhere:
.size strchr,.Lhere-strchr
	.section	.note.GNU-stack,"",@progbits
