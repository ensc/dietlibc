.text

.global cosf,cos,cosl
.type   cosf,@function
.type   cos,@function
.type   cosl,@function

cosf:
        flds    4(%esp)
        jmp .Lcos
cos:
        fldl    4(%esp)
.Lcos:	
	call	__fmod2pi
        fcos
        ret
cosl:
        fldt    4(%esp)
        jmp .Lcos

.Lende:
.size    cos,.Lende-cos
.size    cosl,.Lende-cosl
.size    cosf,.Lende-cosf
