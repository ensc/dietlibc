
.text
.type   asin,@function
.global asin
asin:
        fldl  4(%esp)
        fld   %st(0)
        fmul  %st(0),%st(0)
        fld1
        fsubp %st,%st(1)
        fsqrt
        fpatan
        ret

.ende:
.size    asin,.ende-asin
