/* --*- asm -*-- */

#ifndef H_DIETLIBC_AARCH64_FEATURES_H
#define H_DIETLIBC_AARCH64_FEATURES_H

#define __ARM_ARCH__ 8

#ifdef __ASSEMBLER__

.macro	FUNC_START name
	.text
	.global	\name
	.type	\name, #function
\name:
.endm

.macro	FUNC_START_WEAK name
	.text
	.weak	\name
	.type	\name, #function
\name:
.endm

.macro	FUNC_END name
	.size	\name, . - \name
.endm

/* Needed for a lot of wrappers, so define it here. */
#define ASM_AT_FDCWD     -100

#endif	/* __ASSEMBLER__ */

#endif	/* H_DIETLIBC_AARCH64_FEATURES_H */
