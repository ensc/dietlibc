
.text
.type   strtod,@function
.global strtod
strtod:
#ifdef __DYN_LIB
	pushl	%ebx
	call	1f
1:	popl	%ebx
	addl	$_GLOBAL_OFFSET_TABLE_+[.-1b],%ebx
#endif

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
.Lwhite:incl    %ecx
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
.Lplus: incl    %ecx
.Lnosign:
        push    %edx
        push    %eax


#    while ( (unsigned int)(*p - '0') < 10u )
#        value = value*10 + (*p++ - '0');


        fldz
#ifdef __DYN_LIB
	flds	__ten@GOTOFF(%ebx)					# 10.0     0.0
#else
        flds    __ten                                                   # 10.0     0.0
#endif
.Lint:  movzbl  (%ecx),%eax
        subb    $'0',%al
        cmpb    $10,%al
        jnc     .Lintend
        fmul    %st(0),%st(1)
        movl    %eax,(%esp)
        fild    (%esp)
        incl    %ecx
        faddp   %st(2)
        jmp     .Lint


#    -if ( *p == '.' ) {

.Lintend:                                                               # 10.0     integer

        cmpb    $'.',(%ecx)
        jne     .Lnofrac

#        factor = 1.;
#        p++;
#        while ( (unsigned int)(*p - '0') < 10u ) {
#            factor *= 0.1;
#            value  += (*p++ - '0') * factor;
#        }
#    }

        fld1                                                            # 1.0      10.0         integer
        incl    %ecx
.Lfrac: movzbl  (%ecx),%eax
        subb    $'0',%al
        cmpb    $10,%al
        jnc     .Lfracend
        fdiv    %st(1),%st(0)                                           # 0.1^n    10.0         integer
        movl    %eax,(%esp)
        fld     %st(0)                                                  # 0.1^n    0.1^n        10.0        integer
        fimull  (%esp)                                                  # d*0.1^n  0.1^n        10.0        integer
        faddp   %st(3)                                                  # 0.1^n    10.0         integer+frac
        incl    %ecx
        jmp     .Lfrac

.Lfracend:
        fcomp                                                           # 10.0         integer+frac

.Lnofrac:

#    -if ( (*p | 32) == 'e' ) {

        movb    (%ecx),%al
        orb     $0x20,%al
        cmpb    $'e',%al
        jne     .Lnoexp

#        expo   = 0;
#        factor = 10.L;

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

        incl    %ecx                                                    # 10.0         integer.frac
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

        xorl    %edx,%edx                                               # 10.0 or 0.1   integer.frac

.Lexp:  movzbl  (%ecx),%eax
        subb    $'0',%al
        cmpb    $10,%al
        jnc     .Lexpend
        imull   $10,%edx
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

.Lquad:
        fmul    %st(0),%st(0)
.Lexpend:
        shrl    %edx
        jnc     .Lnomul
        fmul    %st(0),%st(1)
.Lnomul:jnz     .Lquad                                                  # stuff         integer.frac E expo

        fcomp                                                           # integer.frac E expo

.Lnoexp:

#done:
#    -if ( endptr != NULL )
#        *endptr = p;
#
#    return value * sign;
#}

.Lende:
        pop     %eax
        fimull  (%esp)                                                  # +/- integer.frac E expo
        popl    %eax
        movl    8(%esp),%eax
        andl    %eax,%eax
        jz      .Lnostore
        movl    %ecx,(%eax)
.Lnostore:
#ifdef __DYN_LIB
	popl	%ebx
#endif
        ret

.Lend:
.size    strtod,.Lend-strtod
