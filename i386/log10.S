.text
.type   log10,@function
.global log10
log10:
        fldlg2
        fldl    4(%esp)
        fyl2x
        ret

.ende:
.size    log10,.ende-log10
