#include <endian.h>
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

