#
#  Definiert Konstante (float)10. unter dem Namen __ten
#

.global __ten
.data
.type   __ten,@object
.size   __ten,4
__ten:  .long 0x41200000

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
