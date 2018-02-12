#include <endian.h>

#if 0
.data
.type __guard,@object
.global __guard
.type __stack_chk_guard,@object
.global __stack_chk_guard
__guard:
__stack_chk_guard:
#if __WORDSIZE == 64
.align 8
.size	__guard, 8
.quad 0xaff00
#else
.align 4
.size	__guard, 4
.long 0xaff00
#endif

#else

.text
#if __WORDSIZE == 64
.comm __stack_chk_guard,0,8
.comm __guard,8,0
#else
.comm __stack_chk_guard,0,8
.comm __guard,4,0
#endif
.section	.note.GNU-stack,"",@progbits

#endif

