.text
.global htonl
.type   htonl,@function
.global ntohl
.type   ntohl,@function

htonl:
ntohl:
        movl    4(%esp),%eax
        xchgb   %al,%ah
        rorl    $16,%eax
        xchgb   %al,%ah
        ret

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
