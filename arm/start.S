#include "start.h"

#ifdef __arm__

.text
.global _start
_start:
	mov	fp, #0                  /* clear the frame pointer */
	ldr	a1, [sp], #4            /* argc */
	mov	a2, sp                  /* argv */
	ldr	a4, .L3
	add	a3, a2, a1, lsl #2      /* &argv[argc] */
	add	a3, a3, #4              /* envp */
	str	a3, [a4, #0]            /* environ = envp */
	bl	main
	bl	exit

.align 2
.L3:
	.word environ
#endif
