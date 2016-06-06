#include "syscalls.h"

.text
.align 2
.global rt_sigreturn
.type rt_sigreturn,@function
rt_sigreturn:
	mov	$30, $16
	ldi	$0, __NR_rt_sigreturn
	callsys
