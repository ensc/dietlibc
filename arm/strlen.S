#include "dietfeatures.h"
#include "arm-features.h"

.extern	__valgrind

FUNC_START	strlen
#if 0
	teq	a1, #0			@ is string pointer NULL ??
	moveq	pc, lr			@ if so, return 0
#endif

#ifndef WANT_SMALL_STRING_ROUTINES
#ifdef WANT_VALGRIND_SUPPORT
	ldr	ip, =__valgrind
	bne	.Lvalgrind
#endif

	ands	a3, a1, #3
	ldr	ip, .Lmagic
	mov	a2, a1
	ldr	a4, [a1, -a3]!

	/* ensure junk bytes aren't nulls */

	rsbne	a3, a3, #4
	movne	a3, a3, lsl #3
#ifdef __ARMEB__
	orrne	a4, a4, ip, lsl a3
#else
	orrne	a4, a4, ip, lsr a3
#endif

.Lloop:
	sub	a3, a4, ip
	bic	a3, a3, a4
	ands	a3, a3, ip, lsl #7
	ldreq	a4, [a1, #4]!
	beq	.Lloop

	/* a3 & 0x80808080 == a3 */

#ifdef __ARMEB__
	tst	a3, ip, lsl #16 + 7
	moveq	a3, a3, lsl #16
	addeq	a1, a1, #2
	tst	a3, #0x80000000
	addeq	a1, a1, #1
#else
	tst	a3, ip, lsr #16 - 7
	moveq	a3, a3, lsr #16
	addeq	a1, a1, #2
	tst	a3, #0x80
	addeq	a1, a1, #1
#endif
	sub	a1, a1, a2

#ifdef WANT_VALGRIND_SUPPORT
	RET

.Lvalgrind:
#endif
#endif

#if defined(WANT_SMALL_STRING_ROUTINES) || defined(WANT_VALGRIND_SUPPORT)
	ldrb	a3, [a1], #1
	mov	a2, a1
.Lslowloop:
	tst	a3, #255
	ldrneb	a3, [a1], #1
	bne	.Lslowloop
	sub	a1, a1, a2
#endif

	RET

#ifndef WANT_SMALL_STRING_ROUTINES
.Lmagic:
	.word	0x01010101
#endif
FUNC_END	strlen
