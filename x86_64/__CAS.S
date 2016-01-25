.text
.global __CAS
#ifdef __PIE__
.hidden __CAS
#endif
.type __CAS,@function
__CAS:
	movq %rsi, %rax
	lock cmpxchgq %rdx, (%rdi)
	ret
.size __CAS, .-__CAS
