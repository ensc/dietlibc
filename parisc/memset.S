/* memset -- memset(3) -- fill memory buffer

  Copyright (C) 2002 Thomas M. Ogrisegg

  Created:              09/22/02
  Last updated:         09/23/02

  Remarks:
      none
*/

.LEVEL	1.1

.text
.globl memset

memset:
	.PROC
	.CALLINFO
	.ENTRY
	copy %r26, %ret0
	depw,z %r25, 23, 24, %r20
	or %r20, %r25, %r25
	depw,z %r25, 15, 16, %r21
	or %r25, %r21, %r25
	ldi 3, %r19
	and,<> %r26, %r19, %r20
	b,n .Lmdoit
	sub %r19, %r20, %r20
	addi 1, %r20, %r20
	sub %r24, %r20, %r24
.LM3:
	addibf,<,n -1, %r20, .LM3
	stb,ma %r25, 1(%r26)
.Lmdoit:
	addibf,<,n -4, %r24, .Lmdoit
	stw,ma %r25, 4(%r26)
	addi 4, %r24, %r24
.Lnext:
	addibf,<,n -1, %r24, .Lnext
	stb,ma %r25, 1(%r26)
	bv,n %r0(%rp)
	.EXIT
	.PROCEND
	.EXPORT memset
