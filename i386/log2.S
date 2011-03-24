.text
.type   log2,@function
.global log2
log2:
        fld1
        fldl    4(%esp)
        fyl2x
        ret

.ende:
.size    log2,.ende-log2

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
