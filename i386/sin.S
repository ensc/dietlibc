
.text

.global sinf,sin,sinl
.type   sinf,@function
.type   sin,@function
.type   sinl,@function
sinf:
        flds    4(%esp)
	jmp __sin
sin:
        fldl    4(%esp)
__sin:
	call	__fmod2pi
        fsin
        ret
sinl:
        fldt    4(%esp)
        jmp __sin

.ende:
.size    sin,.ende-sin
.size    sinf,.ende-sinf
.size    sinl,.ende-sinl
