.text
.type   log1p,@function
.global log1p
log1p:
        fldln2
        fldl    4(%esp)
        fyl2xp1
        ret

.ende:
.size    log1p,.ende-log1p

