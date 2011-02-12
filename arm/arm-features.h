/* --*- asm -*-- */

#ifndef H_DIETLIBC_ARM_FEATURES_H
#define H_DIETLIBC_ARM_FEATURES_H

/* Stolen from gcc (gcc/config/arm/lib1funcs.asm) */
#if defined(__ARM_ARCH_2__)
# define __ARM_ARCH__ 2
#endif

#if defined(__ARM_ARCH_3__)
# define __ARM_ARCH__ 3
#endif

#if defined(__ARM_ARCH_3M__) || defined(__ARM_ARCH_4__) \
        || defined(__ARM_ARCH_4T__)
# define __ARM_ARCH__ 4
#endif

#if defined(__ARM_ARCH_5__) || defined(__ARM_ARCH_5T__) \
        || defined(__ARM_ARCH_5E__) || defined(__ARM_ARCH_5TE__) \
        || defined(__ARM_ARCH_5TEJ__)
# define __ARM_ARCH__ 5
#endif

#if defined(__ARM_ARCH_6__) || defined(__ARM_ARCH_6J__) \
        || defined(__ARM_ARCH_6K__) || defined(__ARM_ARCH_6Z__) \
        || defined(__ARM_ARCH_6ZK__) || defined(__ARM_ARCH_6T2__) \
        || defined(__ARM_ARCH_6M__)
# define __ARM_ARCH__ 6
#endif

#if defined(__ARM_ARCH_7__) || defined(__ARM_ARCH_7A__) \
        || defined(__ARM_ARCH_7R__) || defined(__ARM_ARCH_7M__)
# define __ARM_ARCH__ 7
#endif

#ifndef __ARM_ARCH__
#error Unable to determine architecture.
#endif



#ifdef __ASSEMBLER__

.macro	FUNC_START name
	.text
	.align	0
	.global	\name
	.type	\name, %function
\name:
.endm

.macro	FUNC_START_WEAK name
	.text
	.align	0
	.weak	\name
	.type	\name, %function
\name:
.endm

.macro	FUNC_END name
	.size	\name, . - \name
.endm

.macro	RET
#if (__ARM_ARCH__ > 4) || defined(__ARM_ARCH_4T__)
	bx	lr
#else
	mov	pc, lr
#endif
.endm

.macro	SWI_UNIFIED	name
#ifdef __ARM_EABI__
	b	__unified_syscall_swi
#else
	swi	\name
	b	__unified_syscall
#endif
.endm

.macro	SWI_UNIFIED4
#ifdef __ARM_EABI__
	b	__unified_syscall_swi
#else
	swi	\name
	b	__unified_syscall4
#endif
.endm

.macro	LOAD_ARG4_5
#if defined(__ARM_ARCH_4__) || defined(__ARM_ARCH_4T__) || defined(__ARM_ARCH_5T__)
	ldr	r4, [sp,#16]
	ldr	r5, [sp,#20]
#else
	ldrd	r4, [sp,#16]
#endif
.endm

#endif	/* __ASSEMBLER__ */

#endif	/* H_DIETLIBC_ARM_FEATURES_H */
