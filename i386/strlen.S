
.text
.type   strlen,@function
.global strlen

strlen:
        movl    4(%esp),%ecx
        xorl    %eax,%eax
        jecxz   .Lnull
        decl    %eax
.Llbl:  incl    %eax
        cmpb    $0,(%ecx, %eax)
        jne     .Llbl
.Lnull: ret

.Lende:

.size    strlen,.Lende-strlen

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
