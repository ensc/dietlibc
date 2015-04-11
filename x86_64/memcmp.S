.text
.global memcmp
.type memcmp,@function
memcmp:	/* rdi=ptr1, esi=ptr2, rdx=bytes */
  cmp $8,%rdx
  jb 1f
  mov (%rdi),%rax
  mov (%rsi),%rcx
  lea 8(%rdi),%rdi
  lea 8(%rsi),%rsi
  bswap %rax
  bswap %rcx
  cmp %rax,%rcx
  lea -8(%rdx),%rdx
  jz memcmp
2:
  seta %al
  setb %ah
  neg %al
  or %ah,%al
  movsbl %al,%eax
  ret
1:
  xor %eax,%eax
  mov %edx,%ecx
  jecxz 1f
  repe cmpsb
  jmp 2b
1:
  ret

.Lhere:
.size memcmp,.Lhere-memcmp
	.section	.note.GNU-stack,"",@progbits
