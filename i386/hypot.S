
.text
.type   hypot,@function
.global hypot
hypot:
        fldl     4(%esp)
        fmul    %st(0),%st(0)
        fldl    12(%esp)
        fmul    %st(0),%st(0)
        faddp
        fsqrt
        ret

.ende:
.size    hypot,.ende-hypot
