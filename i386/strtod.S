.data

.Lten:
        .long  0x41200000

.text
.type   strtod,@function
.global strtod
strtod:

#double  strtod ( const char* s, const char** endptr )
#{
#    register const char*  p     = s;
#    register long double  value = 0.L;
#    int                   sign  = +1;
#    long double           factor;
#    unsigned int          expo;

        movl    4(%esp),%ecx                    # %ecx = p
        xorl    %edx,%edx
        incl    %edx                            # %ebx = sign



#    while ( isspace(*p) )
#        p++;

        decl    %ecx
.Lwhite:  incl    %ecx
        movb    (%ecx),%al
        cmpb    $' ',%al
        je      .Lwhite
        subb    $9,%al
        cmpb    $5,%al
        jc      .Lwhite


#    switch (*p) {
#    case '-': sign = -1;
#    case '+': p++;
#    default : break;
#    }

        cmpb    $'+'-9,%al
        je      .Lplus
        cmpb    $'-'-9,%al
        jne     .Lnosign
        negl    %edx
.Lplus:   incl    %ecx
.Lnosign:
        push    %edx
        push    %eax


#    while ( (unsigned int)(*p - '0') < 10u )
#        value = value*10 + (*p++ - '0');


        fldz
.Lint:    movzbl  (%ecx),%eax
        subb    $'0',%al
        cmpb    $10,%al
        jnc     .Lintend
        fmuls   .Lten
        movl    %eax,(%esp)
        fiaddl  (%esp)
        incl    %ecx
        jmp     .Lint


#    -if ( *p == '.' ) {

.Lintend:

        cmpb    $'.',(%ecx)
        jne     .Lnofrac

#        factor = 1.;
#        p++;
#        while ( (unsigned int)(*p - '0') < 10u ) {
#            factor *= 0.1;
#            value  += (*p++ - '0') * factor;
#        }
#    }

        fld1
        incl    %ecx
.Lfrac:   movzbl  (%ecx),%eax
        subb    $'0',%al
        cmpb    $10,%al
        jnc     .Lfracend
        fdivs   .Lten
        movl    %eax,(%esp)
        fld     %st(0)
        fimull  (%esp)
        faddp   %st(2)
        incl    %ecx
        jmp     .Lfrac

.Lfracend:
        fcomp

.Lnofrac:

#    -if ( (*p | 32) == 'e' ) {

        movb    (%ecx),%al
        orb     $0x20,%al
        cmpb    $'e',%al
        jne     .Lnoexp

#        expo   = 0;
#        factor = 10.L;

        flds    .Lten


#        switch (*++p) {                 // ja hier weiß ich nicht, was mindestens nach einem 'E' folgenden MUSS.
#        case '-': factor = 0.1;
#        case '+': p++;
#                  break;
#        case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
#                  break;
#        default : value = 0.L;
#                  p     = s;
#                  goto done;
#        }

        incl    %ecx
        movb    (%ecx),%al
        subb    $'0',%al
        cmpb    $10,%al
        jc      .Lexpno
        cmpb    $'+'-'0',%al
        je      .Lexpplus
        cmpb    $'-'-'0',%al
        je      .Lexpminus
        fcompp
        fldz
        movl    4+4*2(%esp),%ecx
        jmp     .Lende

.Lexpminus:
        fld1
        fdivp
.Lexpplus:
        incl    %ecx
.Lexpno:

#        while ( (unsigned int)(*p - '0') < 10u )
#            expo = 10 * expo + (*p++ - '0');

        xorl    %edx,%edx

.Lexp:    movzbl  (%ecx),%eax
        subb    $'0',%al
        cmpb    $10,%al
        jnc     .Lexpend
        addl    %edx,%edx
        leal    (%edx,%edx,4),%edx
        addl    %eax,%edx
        incl    %ecx
        jmp     .Lexp

#        while ( 1 ) {
#            -if ( expo & 1 )
#                value *= factor;
#            -if ( (expo >>= 1) == 0 )
#                break;
#            factor *= factor;
#        }
#    }

.Lexpend:
        shrl    $1,%edx
        jnc     .Lnomul
        fmul    %st(0),%st(1)
.Lnomul:  jz      .Lexppop
        fmul    %st(0),%st(0)
        jmp     .Lexpend

.Lexppop:
        fcomp

.Lnoexp:

#done:
#    -if ( endptr != NULL )
#        *endptr = p;
#
#    return value * sign;
#}

.Lende:
        pop     %eax
        fimull  (%esp)
        popl    %eax
        movl    8(%esp),%eax
        andl    %eax,%eax
        jz      .Lnostore
        movl    %ecx,(%eax)
.Lnostore:
        ret

.L_ende:
.size    strtod,.L_ende-strtod
