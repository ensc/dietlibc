/* memcmp -- memcmp(3) -- compare two buffers

  Copyright (C) 2002 Thomas M. Ogrisegg

  Created:		09/22/02
  Last updated:		09/23/02

  Remarks:
      none
*/

.text

memcmp:
	.PROC
	.CALLINFO
	.ENTRY
	ldb,ma 1(%r25), %r19
	ldb,ma 1(%r26), %r20
	addi,ZNV -1, %r24, %r24
	cmpb,=,n %r19, %r20, memcmp
	nop
	bv %r0(%rp)
	sub %r19, %r20, %r28
	.EXIT
	.PROCEND
	.EXPORT memcmp
