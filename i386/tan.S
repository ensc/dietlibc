.text

.global tanf,tan,tanl
.type   tanf,@function
.type   tan,@function
.type   tanl,@function
tanf:
        flds    4(%esp)
	jmp __tan
tan:
        fldl    4(%esp)
__tan:
	call	__fmod2pi
        fsincos
        fdivrp
        ret
tanl:
        fldt    4(%esp)
	jmp __tan

.ende:
.size    tan,.ende-tan
.size    tanf,.ende-tanf
.size    tanl,.ende-tanl
