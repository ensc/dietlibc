
.text
.type   acos,@function
.global acos
acos:
        fldl  4(%esp)
        fld   %st(0)
        fmul  %st(0),%st(0)
        fld1
        fsubp %st,%st(1)
        fsqrt
        fxch  %st(1)
        fpatan
        ret

.ende:
.size    acos,.ende-acos
