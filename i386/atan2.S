
.text
.type   atan2,@function
.global atan2
atan2:
        fldl   4(%esp)
        fldl  12(%esp)
        fpatan
        ret

.ende:
.size    atan2,.ende-atan2
