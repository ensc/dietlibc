.text

.global expf,exp,expl
	.type	 expf,@function
	.type	 exp,@function
	.type	 expl,@function
expf:
	fldl2e
	fmuls 4(%esp)
	jmp .Lfinexp
expl:
	fldl2e
	fldt 4(%esp)
	fmulp
	jmp .Lfinexp
exp:
	fldl2e
	fmull 4(%esp)
.Lfinexp:
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
