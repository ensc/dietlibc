
.text
.type   sincos,@function
.global sincos

# void  sincos ( double x, double* sinx, double* cosx );

sincos:
        fldl     4(%esp)
        fsincos
        movl    16(%esp),%eax
        movl    12(%esp),%ecx
        fstpl   (%eax)
        fstpl   (%ecx)
        ret

.ende:
.size    sincos,.ende-sincos
