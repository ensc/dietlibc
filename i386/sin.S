
.text
.type   sin,@function
.global sin
sin:
        fldl    4(%esp)
        fsin
        ret

.ende:
.size    sin,.ende-sin
