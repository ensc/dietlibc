/*
  Copyright (C) 2002 Thomas M. Ogrisegg

  This is free software. You can redistribute and
  modify it under the terms of the GNU General Public
  Public License.

  memccpy.S
    i386 assembler implementation of memccpy(3)
*/

.text
.globl memccpy
memccpy:
		pushl %esi
		pushl %edi

		movl %esp, %ecx
		movl 0x0c(%ecx), %edi
		movl 0x10(%ecx), %esi
		movb 0x14(%ecx), %dl
		movl 0x18(%ecx), %ecx
		cld
.Lloop:
		lodsb
		stosb
		cmp %al, %dl
		jz .Lout
		decl %ecx
		jnz .Lloop
		xorl %edi, %edi
.Lout:
		movl %edi, %eax
		popl %edi
		popl %esi
		ret
