
.text
.type   atoi,@function
.type   atol,@function
.global atoi
.global atol

atoi:
atol:
        movl    4(%esp),%ecx
        push    %edi

        xorl    %edi,%edi               # sign = +1
        incl    %edi

        decl    %ecx
.Lspaces:
        incl    %ecx                    # while ( isspace(*p) ) p++;
        movb    (%ecx),%al
        cmpb    $' ',%al
        je      .Lspaces
        subb    $9,%al
        cmpb    $5,%al
        jc      .Lspaces                # c = *p - 9;

        cmpb    $'+'-9,%al              # if ( c == '+' - 9 ) p++;
        je      .Lpos
        cmpb    $'-'-9,%al              # else if ( c == '-' - 9 ) sign = -sign, p++;
        jne     .Lnosign
        neg     %edi
.Lpos:  incl    %ecx
.Lnosign:

        xorl    %edx,%edx               # value = 0;
.Lcont: movzbl  (%ecx),%eax             # while ( (unsigned) (*p - '0') < 10 )
        inc     %ecx
        subb    $'0',%al
        cmpb    $10,%al
        jnc     .Lfini
        imull   $10,%edx                #     value = 10*value + (*p - '0');
        addl    %eax,%edx
        jmp     .Lcont

.Lfini: xchg    %edx,%eax               # return value * sign;
        imul    %edi,%eax

        pop     %edi
        ret

.Lende:

.size    atoi,.Lende-atoi
.size    atol,.Lende-atol

