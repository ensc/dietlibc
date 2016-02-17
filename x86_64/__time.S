#include "dietfeatures.h"
#include "syscalls.h"

.text
.global time
#ifdef __PIE__
.hidden time
#endif
.type time,@function
time:
	sub $24, %rsp
	mov %rdi, 16(%rsp)
	xor %rsi, %rsi
	mov %rsp, %rdi
#ifdef __PIC__
	call gettimeofday@PLT
#else
	call gettimeofday
#endif
	mov (%rsp), %rax
	mov 16(%rsp), %rcx
	test %rcx, %rcx
	jz 1f
	mov %rax, (%rcx)
1:
	add $24, %rsp
	ret
.Lhere2:
	.size	 time,.Lhere2-time
	.section	.note.GNU-stack,"",@progbits
