
.text
.type   cos,@function
.global cos
cos:
        fldl    4(%esp)
        fcos
        ret

.Lende:
.size    cos,.Lende-cos
