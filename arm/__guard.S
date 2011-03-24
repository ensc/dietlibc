.data
.align 2
.type __guard,#object
.global __guard
.type __stack_chk_guard,#object
.global __stack_chk_guard
__guard:
__stack_chk_guard:
.long 0xaff00

.size __guard, . - __guard
.size __stack_chk_guard, . - __stack_chk_guard

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
