/* strcpy -- strcpy(3) -- Copy a string

  Copyright (C) 2002 Thomas M. Ogrisegg

  Created:              09/22/02
  Last updated:         09/23/02

  Remarks:
      All copies are done in word-mode. This might result in a
      buffer overflow if the destination buffer is too short.
*/

.LEVEL	1.1

.text

strcpy:
	.PROC
	.CALLINFO
	.ENTRY
	copy %r26, %r28
	ldi 3, %r19
	and,<> %r26, %r19, %r20
	b,n .Lstrcpy2
	sub %r19, %r20, %r20
.Lstrcpy1:
	addi -1, %r20, %r20
	cmpb,< %r0, %r20, .Lstrcpy2
	ldb,ma 1(%r25), %r19
	cmpb,<> %r0, %r19, .Lstrcpy1
	stb,ma %r19, 1(%r26)
	bv,n %r0(%rp)
.Lstrcpy2:
	ldw,ma 4(%r25), %r19
	stbys,b,m %r19, 4(%r26)
	uxor,sbz %r19, %r0, %r0
	b,n .Lstrcpy2
	bv,n %r0(%rp)
	.EXIT
	.PROCEND
	.EXPORT strcpy
