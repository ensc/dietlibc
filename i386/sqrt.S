
.text
.type   sqrt,@function
.global sqrt
sqrt:
        fldl    4(%esp)
        fsqrt
        ret

.ende:
.size    sqrt,.ende-sqrt
