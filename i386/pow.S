.text
.global pow
.type   pow,@function

#
#  double  pow ( double mant, double exp );
#

pow:    fld1                                    # 1
        fldl     4(%esp)                        # mant          1
	
        ftst                                    # mant          1
        fnstsw  %ax
        andb    $0x41,%ah
        cmpb    $0x40,%ah
        fldl    12(%esp)                        # expo          mant            1
        jz      .Lpow0
	
        pushl   %eax
        fistl   (%esp)                          # expo          mant          1
        ficompl (%esp)                          # mant          1
        fnstsw  %ax                             # exponent ganzzahlig?
        andb    $0x44,%ah
        cmpb    $0x40,%ah
        popl    %ecx
        jz      .Lipow                          # ganzzahlig!

                                         
        fyl2x                                   # log2(mant)
        fmull   12(%esp)                        # expo*log2(mant) = x
        fld      %st(0)                         # x              x
        frndint                                 # int(x)         x
        fxch                                    # x              int(x)
        fsub     %st(1),%st(0)                  # frac(x)        int(x)
        f2xm1                                   # 2^(fract(x))-1 int(x)
        fld1                                    # 1              2^(fract(x))-1         int(x)
        faddp    %st(0),%st(1)                  # 2^(fract(x))   int(x)
        fscale                                  # 2^x            int(x)
        fstp     %st(1)                         # 2^x
        ret

###############################################################################

.Lpow0:                                         # expo		0             1
        ftst                                    # expo          0             1
        fnstsw  %ax
        andb    $0x41,%ah
        jz      .Lpow0p
        cmpb    $0x40,%ah
        jz      .Lpow00
.Lpow0n:fcomp                                   # 0             1
        fdivrp                                  # inf
        ret
   

.Lipow: andl    %ecx,%ecx                       # mant          1
        jns     .Lstart
        negl    %ecx
        fdivr   %st(1),%st(0)
        jmp     .Lstart

.Lnext: fmul    %st(0),%st(0)                   # mant^(2^x)    result
.Lstart:shrl    %ecx
        jnc     .Lnomul
        fmul    %st(0),%st(1)
.Lnomul:jnz     .Lnext
        fcomp                                   # result
        ret

################################################################################

.Lpow00:
        fcompp                                  # 1
        ret

.Lpow0p:
        fcomp                                   # 0             1
        fstp    %st(1)                          # 0
        ret


