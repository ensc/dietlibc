/*
  Copyright (C) 2002 Thomas M. Ogrisegg

  This is free software. You can redistribute and
  modify it under the terms of the GNU General Public
  Public License.

  strncpy.S
    i386 assembler implementation of strncpy(3)
*/

#include "dietfeatures.h"

.text
.globl strncpy
.type strncpy,@function

strncpy:
	pushl %esi
	pushl %edi
	movl %esp, %ecx
	movl  0x0c(%ecx), %edi
	movl  0x10(%ecx), %esi
	movl  0x14(%ecx), %ecx

	movl %edi, %edx
	cld
.Lloop:
	dec %ecx
	js .Lout
	lodsb
	stosb
	or %al, %al
	jnz .Lloop
#ifdef WANT_FULL_POSIX_COMPAT
	repnz stosb
#endif
.Lout:
	movl %edx, %eax
	popl %edi
	popl %esi
	ret
