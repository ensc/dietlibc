/*
  Copyright (C) 2002 Thomas M. Ogrisegg

  This is free software. You can redistribute and
  modify it under the terms of the GNU General Public
  Public License.

  strncpy.S
    i386 assembler implementation of strncpy(3)
*/

.text
.globl strncpy

strncpy:
		pushl %esi
		pushl %edi
		movl %esp, %ecx
		movl  0x0c(%ecx), %edi
		movl  0x10(%ecx), %esi
		movl  0x14(%ecx), %ecx

		movl %edi, %edx

.Lloop:
		dec %ecx
		js .Lout
		lodsb
		stosb
		or %al, %al
		jnz .Lloop
.Lout:
#ifdef WANT_FULL_POSIX_COMPAT
		xorl %eax, %eax
		repnz stosb
#endif
		movl %edx, %eax
		popl %edi
		popl %esi
		ret
