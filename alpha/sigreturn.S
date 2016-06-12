#include "syscalls.h"

.text
.align 2
.global sigreturn
.type sigreturn,@function
sigreturn:
	mov	$30, $16
	ldi	$0, __NR_sigreturn
	callsys

