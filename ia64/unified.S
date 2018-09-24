#include <dietfeatures.h>

/*
   Copyright (C) 2002 Thomas M. Ogrisegg

  This is free software. You can redistribute and
  modify it under the terms of the GNU General Public
  Public License.

  This file is part of the ia64-Port of dietlibc

  unified.S
    General system-call interface
*/

.lsb

.text

.globl __unified_syscall
.globl __error_unified_syscall
.globl _exit
.weak exit
.globl _Exit
.proc  _exit
.proc  _Exit

_exit:
_Exit:
exit:
	mov r15 = 1025
.endp _exit
.size _exit, . - _exit

.proc  __unified_syscall
__unified_syscall:
	break.i 0x100000
#ifdef WANT_THREAD_SAFE
	addl r2 = @ltoff(@tprel(errno#)), r1
	;;
	ld8 r2 = [r2]
	;;
	add r2 = r2, r13
	;;
#else
	movl r2=errno
#endif
	cmp.eq p6,p0=-1,r10
	;;
.endp __unified_syscall
.size __unified_syscall, . - __unified_syscall

.proc  __error_unified_syscall
__error_unified_syscall:
(p6)    st4 [r2]=r8
(p6)    mov r8=-1

#include "dietuglyweaks.h"

	br.ret.sptk.few rp
.endp __error_unified_syscall

.size __error_unified_syscall, . - __error_unified_syscall
