
.text
.type   cos,@function
.global cos
cos:
        fldl    4(%esp)
        fcos
        ret

.ende:
.size    cos,.ende-cos
