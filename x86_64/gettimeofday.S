.text
.global gettimeofday
.type gettimeofday,@function
gettimeofday:
	mov	$0xffffffffff600000,%rax
	callq	*%rax
	jmp __error_unified_syscall
.Lhere:
	.size	 gettimeofday,.Lhere-gettimeofday
