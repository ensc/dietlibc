#include "dietfeatures.h"

#ifdef __arm__

.text
.global _start
_start:
#ifdef WANT_DYNAMIC
	mov	a4, a1			/* save dynamic ld.so _fini */
#endif
	mov	fp, #0                  /* clear the frame pointer */
	ldr	a1, [sp], #4            /* argc */
	mov	a2, sp                  /* argv */
	ldr	ip, .L3
	add	a3, a2, a1, lsl #2      /* &argv[argc] */
	add	a3, a3, #4              /* envp */
	str	a3, [ip, #0]            /* environ = envp */
#ifdef WANT_DYNAMIC
	bl	_dyn_start
#else
	bl	main
#endif
	bl	exit

.align 2
.L3:
	.word environ
#endif
