#include "dietfeatures.h"

#ifdef __arm__

.text
.global _start
_start:
#ifdef WANT_DYNAMIC
	mov	a4, a1			/* save dynamic ld.so _fini */
#endif
	mov	fp, #0			/* clear the frame pointer */
	ldr	a1, [sp], #4		/* argc */
	mov	a2, sp			/* argv */
	ldr	ip, .L3
	add	a3, a2, a1, lsl #2	/* &argv[argc] */
	add	a3, a3, #4		/* envp */
#ifdef __DYN_LIB
	ldr	sl, .L4
1:	add	sl, pc, sl
	str	a3, [sl, ip]		/* environ = envp */
#else
	str	a3, [ip, #0]		/* environ = envp */
#endif
#ifdef WANT_DYNAMIC
	bl	_dyn_start
#else
	bl	main
#endif
	bl	exit

.align 2
.L3:
#ifdef __DYN_LIB
	.word environ(GOT)
.L4:
	.word _GLOBAL_OFFSET_TABLE_-(1b+8)
#else
	.word environ
#endif

#endif
