.text

.global cosf,cos,cosl
.type   cosf,@function
.type   cos,@function
.type   cosl,@function

cosf:
        flds    4(%esp)
        jmp __cos
cos:
        fldl    4(%esp)
__cos:	
	call	fmod2pi
        fcos
        ret
cosl:
        fldt    4(%esp)
        jmp __cos

.Lende:
.size    cos,.Lende-cos
.size    cosl,.Lende-cosl
.size    cosf,.Lende-cosf
