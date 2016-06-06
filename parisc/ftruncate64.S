#include "parisc/syscalls.h"

.text
.type __dietlibc_ftruncate64,@function
.globl __dietlibc_ftruncate64
.export __dietlibc_ftruncate64
__dietlibc_ftruncate64:
#if !defined(__LP64__)
	copy %arg3,%arg1
#endif
	b __unified_syscall
	ldi __NR_ftruncate64, %r20
