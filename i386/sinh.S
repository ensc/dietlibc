
.text
.type   sinh,@function
.global sinh

sinh:                           # note: exp(x) = 2^(x*log2(e))
        fldl2e
        fmull    4(%esp)        # x*log2(e)
        fld      %st(0)         # x*log2(e)              x*log2(e)
        frndint                 # int(x*log2(e))         x*log2(e)
        fxch                    # x*log2(e)              int(x*log2(e))
        fsub     %st(1),%st(0)  # frac(x*log2(e))        int(x*log2(e))
        f2xm1                   # 2^(fract(x*log2(e)))-1 int(x*log2(e))
        fld1                    # 1                      2^(fract(x*log2(e)))-1         int(x*log2(e))
        faddp    %st(0),%st(1)  # 2^(fract(x*log2(e)))   int(x*log2(e))
        fscale                  # 2^(x*log2(e))          int(x*log2(e))
        fstp     %st(1)         # 2^(x*log2(e))

# now we have y = exp(x), but sinh(x) = (y - 1/y) * 0.5

        fld1
        fdiv    %st(1),%st(0)
        fsubrp
        fmuls   __half
        ret

.Lende:

.size    sinh,.ende-sinh
