.text

.global expf,exp,expl,__finexp
	.type	 expf,@function
	.type	 exp,@function
	.type	 expl,@function
	.type	 __finexp,@function
expf:
	fldl2e
	fmuls 4(%esp)
	jmp __finexp
expl:
	fldl2e
	fldt 4(%esp)
	fmulp
	jmp __finexp
exp:
	fldl2e
	fmull 4(%esp)
__finexp:
	fst	%st(1)
	frndint
	fst	%st(2)
	fsubrp
	f2xm1
	fld1
	faddp
	fscale
	ret

.Lende:
.size    exp,.Lende-exp
.size    expl,.Lende-expl
.size    expf,.Lende-expf
.size    __finexp,.Lende-__finexp
