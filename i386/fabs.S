
.text
.type   fabs,@function
.global fabs
fabs:
        fldl    4(%esp)
        fabs
        ret

.ende:
.size    fabs,.ende-fabs
