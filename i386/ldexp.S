.text
.type   ldexp,@function
.global ldexp

# double  ldexp ( double value, int expo );

ldexp:
        fildl 12(%esp)
        fldl   4(%esp)
        fscale
        fstp  %st(1)
        ret

.ende:
.size    ldexp,.ende-ldexp
