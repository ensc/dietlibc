/*
    Copyright (C) 2003 Thomas M. Ogrisegg <tom@hi-tek.fnord.at>

    This is free software. You can redistribute and
    modify it under the terms of the GNU General Public
    Public License.

    memccpy.S
      sparc assembler implementation of memccpy(3)
*/

.text
.globl memccpy
memccpy:
	clr %o4
.Lloop:
	subcc %o3, 1, %o3
	bl,a .Lret
	sub %g0, %o4, %o0
	ldub [%o1+%o4], %o5
	cmp %o5, %o2
	inc %o4
	bne,a .Lloop
	stb %o5, [%o0+%o4]
.Lret:
	retl
	add %o0, %o4, %o0
