
#include "dietfeatures.h"
#include "syscalls.h"

	.text
	.align 2

	.global _start
	.weak 	exit
	.global _exit

_start:

#ifdef WANT_DYNAMIC
	mov	a4, a1			@ save dynamic ld.so _fini
#endif
	mov	fp, #0			@ clear the frame pointer
	ldr	a1, [sp], #4		@ argc
	mov	a2, sp			@ argv
	ldr	ip, .L3
	add	a3, a2, a1, lsl #2	@ &argv[argc]
	add	a3, a3, #4		@ envp
#ifdef __DYN_LIB
	ldr	sl, .L4
1:	add	sl, pc, sl
	str	a3, [sl, ip]		@ environ = envp
#else
	str	a3, [ip, #0]		@ environ = envp
#endif

#ifdef WANT_DYNAMIC
	bl	_dyn_start
#else
	bl	main
#endif

@
@ The exit status from main() is already in r0.
@ All we need to do now is drop through to the exit syscall.
@

exit:
_exit:
	swi	$__NR_exit		@ never returns.

#ifdef __DYN_LIB
.L3:	.word 	environ(GOT)
.L4:	.word 	_GLOBAL_OFFSET_TABLE_-(1b+8)
#else
.L3:	.word 	environ
#endif
