/*
  Copyright (C) 2001, 2002 Thomas M. Ogrisegg

  This is free software. You can redistribute and
  modify it under the terms of the GNU General Public
  Public License.

  This file is part of the hppa-Port of dietlibc

  unified.S
    General system-call interface
*/

.level 1.1

.text

.export __unified_syscall
.global __unified_syscall
.type __unified_syscall,@function

__unified_syscall:
	be,l 0x100(%sr2, %r0), %sr0, %r31
	nop
	ldi -0x100, %r1
	cmpb,>>=,n %r1, %ret0, .Lnoerr
;	combf,>,n %r1, %ret0, .Lnoerr ; optimize with ldi -0x100
	sub %r0, %ret0, %ret0
	ldil LP%errno, %r26
	ldo  RP%errno(%r26), %r26
	stw  %ret0, 0(%r0, %r26)
	ldi -1, %ret0
	nop
.Lnoerr:

/* here we go and "reuse" the return for weak-void functions */
#include "dietuglyweaks.h"

	bv,n %r0(%rp)
	nop
