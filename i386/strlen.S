
.text
.type   strlen,@function
.global strlen

strlen:
        movl    4(%esp),%ecx
        xorl    %eax,%eax
        jecxz   .Lnull
        decl    %ecx
        decl    %eax
.Llbl:  incl    %eax
        incl    %ecx
        cmpb    $0,(%ecx)
        jne     .Llbl
.Lnull: ret

.Lende:

.size    strlen,.Lende-strlen
