.text
.type   log,@function
.global log
log:
        fldln2
        fldl    4(%esp)
        fyl2x
        ret

.ende:
.size    log,.ende-log
