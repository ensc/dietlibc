/* memcpy -- memcpy(3) -- copy a memory area

  Copyright (C) 2002 Thomas M. Ogrisegg

  Created:              09/22/02
  Last updated:         09/23/02

  Remarks:
      none
*/

.LEVEL	1.1

.text
.globl memcpy
memcpy:
	.PROC
	.CALLINFO
	.ENTRY
	copy %r26, %ret0
	ldi 3, %r19
	and,<> %r25, %r19, %r20
	b,n .Lmemcpy_do_it
	sub %r19, %r20, %r20
.LM3:
	ldb,ma 1(%r25), %r19
	stb,ma %r19, 1(%r26)
	cmpb,<> %r0, %r20, .LM3
	addi -1, %r20, %r20
.Lmemcpy_do_it:
	ldw,ma 4(%r25), %r19
	stbys,b,m %r19, 4(%r26)
	addi -4, %r24, %r24
	cmpib,<<=,n 3, %r24, .Lmemcpy_do_it
	cmpb,=,n %r0, %r24, .Lret
.LM5:
	ldb,ma 1(%r25), %r19
	stb,ma %r19, 1(%r26)
	cmpb,<> %r0, %r24, .LM5
	addi -1, %r24, %r24
.Lret:
	bv,n %r0(%rp)
	.EXIT
	.PROCEND
	.EXPORT memcpy
