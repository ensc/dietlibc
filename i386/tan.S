
.text
.type   tan,@function
.global tan
tan:
        fldl    4(%esp)
        fsincos
        fdivrp
        ret

.Lende:
.size    tan,.Lende-tan
