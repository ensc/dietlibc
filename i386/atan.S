
.text
.type   atan,@function
.global atan
atan:
        fldl  4(%esp)
        fld1
        fpatan
        ret

.ende:
.size    atan,.ende-atan
