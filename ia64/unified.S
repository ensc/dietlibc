/*
   Copyright (C) 2002 Thomas M. Ogrisegg

  This is free software. You can redistribute and
  modify it under the terms of the GNU General Public
  Public License.

  This file is part of the hppa-Port of dietlibc

  unified.S
    General system-call interface
*/

.lsb

.text

.globl __unified_syscall
.proc  __unified_syscall
.globl __error_unified_syscall
.proc  __error_unified_syscall
.globl _exit
.proc  _exit

_exit:
	mov r0 = 1025
.endp _exit
.size _exit, . - _exit

__unified_syscall:
	break.i 0x100000
	cmp.eq p6,p0=-1,r10
	;;
__error_unified_syscall:
(p6)	addl r33  = @ltoff(errno), gp
(p6)	ld8  r33  = [r33]
(p6)	st4 [r33] = r8
(p6)	mov r8 = -1

#include "dietuglyweaks.h"

	br.ret.sptk.few rp

.endp __unified_syscall
.endp __error_unified_syscall
.size __unified_syscall, __error_unified_syscall - __unified_syscall
.size __error_unified_syscall, . - __error_unified_syscall
