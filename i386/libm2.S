.text
#d.half:
#	.long	0x3f000000	# 1/2
#d.log1p:
#	.long	0x3ed413cc	# < sqrt(2)-1-2^(-25)

.global sqrtf,sqrt,sqrtl
	.type	 sqrtf,@function
	.type	 sqrt,@function
	.type	 sqrtl,@function
sqrtf:
	flds 4(%esp)
	fsqrt
	ret
sqrt:
	fldl 4(%esp)
	fsqrt
	ret
sqrtl:
	fldt 4(%esp)
	fsqrt
	ret

.global fabsf,fabs,fabsl
	.type	fabsf,@function
	.type	fabs,@function
	.type	fabsl,@function
fabsf:
	flds 4(%esp)
	fabs
	ret
fabs:
	fldl 4(%esp)
	fabs
	ret
fabsl:
	fldt 4(%esp)
	fabs
	ret

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
	call	fmod2pi
        fsin
        ret
sinl:
        fldt    4(%esp)
        jmp __sin

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

.global sincosf,sincos,sincosl
.type   sincosf,@function
.type   sincos,@function
.type   sincosl,@function
# void  sincos ( double x, double* sinx, double* cosx );

sincosf:
        flds     4(%esp)
	call	fmod2pi
        movl    12(%esp),%eax
        movl    8(%esp),%ecx
        fsincos
        fstps   (%eax)
        fstps   (%ecx)
        ret
sincos:
        fldl     4(%esp)
	call	fmod2pi
        movl    16(%esp),%eax
        movl    12(%esp),%ecx
        fsincos
        fstpl   (%eax)
        fstpl   (%ecx)
        ret
sincosl:
        fldt     4(%esp)
	call	fmod2pi
        movl    20(%esp),%eax
        movl    16(%esp),%ecx
        fsincos
        fstpt   (%eax)
        fstpt   (%ecx)
        ret

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
	call	fmod2pi
        fsincos
        fdivrp
        ret
tanl:
        fldt    4(%esp)
	jmp __tan

.global acosf,acos,acosl	# ,__acos
	.type	 acosf,@function
	.type	 acos,@function
	.type	 acosl,@function
#	.type	 __acos,@function
acosf:
	flds 4(%esp)
	jmp __acos
acosl:
	fldt 4(%esp)
	jmp __acos
acos:
	fldl 4(%esp)
__acos:
	fld	%st
	fmul	%st
	fld1
	fsubp
	fsqrt
	fxch	%st(1)
	fpatan
	ret

.global asinf,asin,asinl	# ,__asin
	.type	 asinf,@function
	.type	 asin,@function
	.type	 asinl,@function
#	.type	 __asin,@function
asinf:
	flds 4(%esp)
	jmp __asin
asinl:
	fldt 4(%esp)
	jmp __asin
asin:
	fldl 4(%esp)
__asin:
	fld	%st
	fmul	%st
	fld1
	fsubp
	fsqrt
	fpatan
	ret

.global atan2f,atan2,atan2l
	.type	 atan2f,@function
	.type	 atan2,@function
	.type	 atan2l,@function
atan2f:
	flds 4(%esp)
	flds 8(%esp)
	fpatan
	ret
atan2l:
	fldt 4(%esp)
	fldt 16(%esp)
	fpatan
	ret
atan2:
	fldl 4(%esp)
	fldl 12(%esp)
	fpatan
	ret

.global atanf,atan,atanl
	.type	 atanf,@function
	.type	 atan,@function
	.type	 atanl,@function
atanf:
	flds 4(%esp)
	fld1
	fpatan
	ret
atan:
	fldl 4(%esp)
	fld1
	fpatan
	ret
atanl:
	fldt 4(%esp)
	fld1
	fpatan
	ret

.global cbrtf,cbrt,cbrtl
	.type	cbrtf,@function
	.type	cbrt,@function
	.type	cbrtl,@function
cbrtf:
	flds 4(%esp)
	jmp __cbrt
cbrtl:
	fldt 4(%esp)
	jmp __cbrt
cbrt:
	fldl 4(%esp)
# fldt 1/3
__cbrt:
	pushl $0x00003ffd	# yes, this method of loading 1/3
	pushl $0xaaaaaaaa	# is shorter than keeping the data
	pushl $0xaaaaaaab	# separate
	fldt (%esp)
	addl $12,%esp
	fxch			# st(0)=x, st(1)=1/3
	ftst
	fstsw %ax
	sahf
	jz 1f
	jnc finpow
	fchs
	call finpow
	fchs
1:	ret

# pow(x,y)
.global powf,pow,powl
	.type	powf,@function
	.type	pow,@function
	.type	powl,@function
powf:
	flds 4(%esp)	# x
	flds 8(%esp)	# y
	jmp __pow
powl:
	fldt 4(%esp)
	fldt 16(%esp)
	jmp __pow
pow:
	fldl 4(%esp)
	fldl 12(%esp)
# x^y; st(0)=y, st(1)=x
__pow:
	ftst		# y = 0 ?
	fstsw %ax
	fld1		# st(0)=1, st(1)=y, st(2)=x
	sahf
	jz 1f		# return 1
	fcomp %st(1)	# y = 1 ?
	fstsw %ax
	fxch		# st(0)=x, st(1)=y
	sahf
	jz 1f		# return x
	ftst		# x = 0 ?
	fstsw %ax
	sahf
	jz 1f
	jnc finpow	# x > 0
	fxch		# st(0)=y, st(1)=x
	fld %st(0)	# st(0)=y, st(1)=y, st(2)=x
	frndint		# st(0)=int(y)
	fcomp %st(1)	# y = int(y)?
	fstsw %ax
	fxch
	sahf
	jnz finpow	# fyl2x -> st(0) = NaN
# y even or odd ?
	fld1
	fadd %st(0)	# st(0) = 2
	fdivr %st(2),%st(0) # st(0)=st(2)/2
	frndint
	fadd %st(0),%st(0)
	fcomp %st(2)	# st(0) = x, st(1) = y
	fstsw %ax
	fchs		# st(0) = -x
	sahf
	jz finpow	# y even
	call finpow	# y odd
	fchs
1:	ret

# x^y; st(0)=x, st(1)=y (x > 0)
finpow:
	fyl2x
	jmp finexp

.global exp2f,exp2,exp2l
	.type	 exp2f,@function
	.type	 exp2,@function
	.type	 exp2l,@function
exp2f:
	flds 4(%esp)
	jmp finexp
exp2:
	fldl 4(%esp)
	jmp finexp
exp2l:
	fldt 4(%esp)
	jmp finexp

.global expf,exp,expl
	.type	 expf,@function
	.type	 exp,@function
	.type	 expl,@function
expf:
	fldl2e
	fmuls 4(%esp)
	jmp finexp
expl:
	fldl2e
	fldt 4(%esp)
	fmulp
	jmp finexp
exp:
	fldl2e
	fmull 4(%esp)
finexp:
	fst	%st(1)
	frndint
	fst	%st(2)
	fsubrp
	f2xm1
	fld1
	faddp
	fscale
	ret

.global exp10f,exp10,exp10l	#,pow10f,pow10,pow10l
	.type	 exp10f,@function
	.type	 exp10,@function
	.type	 exp10l,@function
#	.type	 pow10f,@function
#	.type	 pow10,@function
#	.type	 pow10l,@function
exp10f:
#pow10f:
	fldl2t
	fmuls 4(%esp)
	jmp finexp
exp10:
#pow10:
	fldl2t
	fmull 4(%esp)
	jmp finexp
exp10l:
#pow10l:
	fldl2t
	fldt 4(%esp)
	fmulp
	jmp finexp

# exp(x)-1
.global expm1f,expm1,expm1l
	.type	 expm1f,@function
	.type	 expm1,@function
	.type	 expm1l,@function
expm1f:
	fldl2e
	fmuls 4(%esp)
	jmp finem1
expm1l:
	fldl2e
	fldt 4(%esp)
	fmulp
	jmp finem1
expm1:
	fldl2e
	fmull 4(%esp)
# -1 <= st <= 1 ?
finem1:
	fst %st(1)	# st(1)=st(0)
	fabs
	fld1
	fcompp
	fstsw %ax
	sahf
# |x| >= 1
	jc 1f
	f2xm1
	ret
1:	call finexp
	fld1
	fsubrp
	ret

# sinh(x)=(exp(x)-exp(-x))/2
# cosh(x)=(exp(x)+exp(-x))/2
# tanh(x)=sinh(x)/cosh(x)
.global sinhf,sinh,sinhl
	.type	sinhf,@function
	.type	sinh,@function
	.type	sinhl,@function
sinhf:
	fldl2e
	fmuls 4(%esp)
	jmp finsinh
sinh:
	fldl2e
	fmull 4(%esp)
finsinh:
	call finexp
	fld1
	fdiv %st(1),%st(0)	# st(0)=1/exp(x), st(1)=exp(x)
	fsubrp %st(0),%st(1)
	pushl $0x3f000000	# 1/2
	flds (%esp)
	popl %eax
	fmulp
	ret

sinhl:
	fldl2e
	fldt 4(%esp)
	fmulp
	jmp finsinh

.global coshf,cosh,coshl
	.type	coshf,@function
	.type	cosh,@function
	.type	coshl,@function
coshf:
	fldl2e
	fmuls 4(%esp)
	jmp fincosh
cosh:
	fldl2e
	fmull 4(%esp)
fincosh:
	call finexp
	fld1
	fdiv %st(1),%st(0)	# st(0)=1/exp(x), st(1)=exp(x)
	faddp %st,%st(1)
	pushl $0x3f000000	# 1/2
	flds (%esp)
	popl %eax
	fmulp
	ret

coshl:
	fldl2e
	fldt 4(%esp)
	fmulp
	jmp fincosh

.global tanhf,tanh,tanhl
	.type	tanhf,@function
	.type	tanh,@function
	.type	tanhl,@function
tanhf:
	fldl2e
	fmuls 4(%esp)
	call finexp
	jmp fintanh
tanh:
	fldl2e
	fmull 4(%esp)
	call finexp
fintanh:
	fld1
	fdiv %st(1),%st	# st(0)=1/exp(x), st(1)=exp(x)
	fst %st(2)	# st(2)=1/exp(x)
	fadd %st(1),%st(0)
	fstp %st(3)	# st(2)=exp(x)+exp(-x), st(1)=exp(-x), st(0)=exp(x)
	fsubp		# st(1)=exp(x)+exp(-x), st(0)=exp(x)-exp(-x)
	fdivp
	ret

tanhl:
	fldl2e
	fldt 4(%esp)
	fmulp
	call finexp
	jmp fintanh

.global hypotf,hypot,hypotl	# ,__hypot
	.type	 hypotf,@function
	.type	 hypot,@function
	.type	 hypotl,@function
#	.type	 __hypot,@function
hypotf:
	flds 8(%esp)
	flds 4(%esp)
	jmp __hypot
hypotl:
	fldt 16(%esp)
	fldt 4(%esp)
	jmp __hypot
hypot:
	fldl 12(%esp)
	fldl 4(%esp)
__hypot:
	fmul %st(0),%st(0)
	fxch
	fmul %st(0),%st(0)
	faddp
	fsqrt
	ret

.global logf,log,logl
	.type	 logf,@function
	.type	 log,@function
	.type	 logl,@function
logf:
	fldln2
	flds 4(%esp)
	fyl2x
	ret
logl:
	fldln2
	fldt 4(%esp)
	fyl2x
	ret
log:
	fldln2
	fldl 4(%esp)
	fyl2x
	ret

.global log1pf,log1p,log1pl
	.type	 log1pf,@function
	.type	 log1p,@function
	.type	 log1pl,@function
log1pf:
	flds 4(%esp)
	jmp __log1p
log1pl:
	fldt 4(%esp)
	jmp __log1p
log1p:
	fldl 4(%esp)
__log1p:
# Sprawdzenie zakresu parametru
	fst %st(1)
	pushl	$0x3ed413cc	# sqrt(2)-1-2^(-25)
	fabs
	flds (%esp)
	popl %eax
	fcompp			# porownanie
	fstsw %ax
	fldln2
	fxch
	sahf
# |x| >= sqrt(2)-1
	jc 1f
	fyl2xp1
	ret
1:	fld1		# x = x + 1
	faddp
	fyl2x
	ret

.global log10f,log10,log10l
	.type	 log10f,@function
	.type	 log10,@function
	.type	 log10l,@function
log10f:
	fldlg2
	flds 4(%esp)
	fyl2x
	ret
log10l:
	fldlg2
	fldt 4(%esp)
	fyl2x
	ret
log10:
	fldlg2
	fldl 4(%esp)
	fyl2x
	ret

.global log2f,log2,log2l
	.type	 log2f,@function
	.type	 log2,@function
	.type	 log2l,@function
log2f:
	fld1
	flds 4(%esp)
	fyl2x
	ret
log2l:
	fld1
	fldt 4(%esp)
	fyl2x
	ret
log2:
	fld1
	fldl 4(%esp)
	fyl2x
	ret

.global fmaf,fma,fmal
	.type	fmaf,@function
	.type	fma,@function
	.type	fmal,@function
fmaf:
	flds 4(%esp)
	fmuls 8(%esp)
	fadds 12(%esp)
	ret
fma:
	fldl 4(%esp)
	fmull 12(%esp)
	faddl 20(%esp)
	ret
fmal:
	fldt 4(%esp)
	fldt 16(%esp)
	fmulp %st,%st(1)
	fldt 28(%esp)
	faddp %st,%st(1)
	ret

.global asinhf,asinh,asinhl
	.type	asinhf,@function
	.type	asinh,@function
	.type	asinhl,@function
asinhf:
	flds 4(%esp)
	jmp __asinh
asinh:
	fldl 4(%esp)
__asinh:
	fld %st(0)
	fmul %st(0),%st(0)
	fld1
	faddp %st(0),%st(1)
finasch:
	fsqrt
	faddp %st(0),%st(1)
	fldln2
	fxch
	fyl2x
	ret
asinhl:
	fldt 4(%esp)
	jmp __asinh

.global acoshf,acosh,acoshl
	.type	acoshf,@function
	.type	acosh,@function
	.type	acoshl,@function
acoshf:
	flds 4(%esp)
	jmp __acosh
acosh:
	fldl 4(%esp)
__acosh:
	fld %st(0)
	fmul %st(0),%st(0)
	fld1
	fsubrp %st(0),%st(1)	# st1=st1-st0; pop
	jmp finasch
acoshl:
	fldt 4(%esp)
	jmp __acosh

.global atanhf,atanh,atanhl
	.type	atanhf,@function
	.type	atanh,@function
	.type	atanhl,@function
atanhf:
	flds 4(%esp)
	jmp __atanh
atanh:
	fldl 4(%esp)
__atanh:
	fst %st(1)
	fld1			# st0=1, st1=x, st2=x
	fadd %st(0),%st(2)	# st0=1, st1=x, st2=x+1
	fsubp %st(0),%st(1)	# st0=1-x, st1=x+1
	fdivrp %st(0),%st(1)
	fsqrt
	fldln2
	fxch
	fyl2x
	ret
atanhl:
	fldt 4(%esp)
	jmp __atanh

fmod2pi:
	fldpi
	fadd %st(0)
	fxch
	jmp __fmod

.global fmodf,fmod,fmodl
	.type	fmodf,@function
	.type	fmod,@function
	.type	fmodl,@function
fmodf:
	flds 8(%esp)		# y
	flds 4(%esp)		# x
	jmp __fmod
fmod:
	fldl 12(%esp)
	fldl 4(%esp)
__fmod:
	fprem
	fstsw %ax
	sahf
	jp __fmod
	ret

fmodl:
	fldt 16(%esp)
	fldt 4(%esp)
	jmp __fmod

.global dremf,drem,dreml
	.type	dremf,@function
	.type	drem,@function
	.type	dreml,@function
dremf:
	flds 8(%esp)		# y
	flds 4(%esp)		# x
	jmp __drem
drem:
	fldl 12(%esp)
	fldl 4(%esp)
__drem:
	fprem1
	fstsw %ax
	sahf
	jp __drem
	ret

dreml:
	fldt 16(%esp)
	fldt 4(%esp)
	jmp __drem

# |ret| = |x|, sign(ret) = sign(y)
.global copysignf,copysign,copysignl
	.type	copysignf,@function
	.type	copysign,@function
	.type	copysignl,@function
copysignf:
	flds 4(%esp)		# x
	flds 8(%esp)		# y
	jmp __copysign
copysign:
	fldl 4(%esp)
	fldl 12(%esp)
__copysign:
	fmul %st(1),%st		# st = x*y
	ftst
	fstsw %ax
	fincstp
	sahf
	jnc 1f
	fchs
1:	ret

copysignl:
	fldt 4(%esp)
	fldt 16(%esp)
	jmp __copysign

.global rintf,rint,rintl
	.type	 rintf,@function
	.type	 rint,@function
	.type	 rintl,@function
rintf:
	flds 4(%esp)
	frndint
	ret
rint:
	fldl 4(%esp)
	frndint
	ret
rintl:
	fldt 4(%esp)
	frndint
	ret

.global fdimf,fdim,fdiml
	.type	 fdimf,@function
	.type	 fdim,@function
	.type	 fdiml,@function
fdimf:
	flds 4(%esp)
	fsubl 12(%esp)
	jmp __fdim
fdim:
	fldl 4(%esp)
	fsubl 12(%esp)
__fdim:
	fstsw %ax
	sahf
	jnc 1f
	fldz
1:	ret
fdiml:
	fldt 4(%esp)
	fldt 16(%esp)
	fsubp
	jmp __fdim

.global floorf,floor,floorl
	.type floorf,@function
	.type floor,@function
	.type floorf,@function

floorf:
	flds 4(%esp)
	movb $0x04,%ah
	jmp flcetr

floor:
	fldl 4(%esp)
	movb $0x04,%ah
	jmp flcetr

floorl:
	fldt 4(%esp)
	movb $0x04,%ah
	jmp flcetr

.global ceilf,ceil,ceill
	.type ceilf,@function
	.type ceil,@function
	.type ceill,@function

ceilf:
	flds 4(%esp)
	movb $0x08,%ah
	jmp flcetr

ceill:
	fldt 4(%esp)
	movb $0x08,%ah
	jmp flcetr

ceil:
	fldl 4(%esp)
	movb $0x08,%ah

# Wspolny kod dla funkcji floor, ceil i trunc
# W ah maska bitow 11 i 10 rejestru sterowania koprocesora
flcetr:
	xorl %ecx,%ecx	# wyzerowanie rejestru
	movb %ah,%ch	# i utworzenie maski w cx
	pushl %eax	# krotsze niz subl $4,%esp
	fstcw (%esp)
	movw (%esp),%ax
	andb $0x03,%ah	# wyzerowanie bitow 11 i 10
	orl %ecx,%eax	# ustawienie bitow z maski
	movw %ax,2(%esp)
	fldcw 2(%esp)
	frndint
	fldcw (%esp)	# odtworzenie rejestru sterowania
	popl %eax	# i polozenia stosu
	ret

.global truncf,trunc,truncl
	.type truncf,@function
	.type trunc,@function
	.type truncl,@function

truncf:
	flds 4(%esp)
	movb $0x0c,%ah
	jmp flcetr

trunc:
	fldl 4(%esp)
	movb $0x0c,%ah
	jmp flcetr

truncl:
	fldt 4(%esp)
	movb $0x0c,%ah
	jmp flcetr

.global frexpf,frexp,frexpl
	.type frexpf,@function
	.type frexp,@function
	.type frexpl,@function

frexpf:
	flds 4(%esp)
	movl 8(%esp),%eax
	jmp __frexp

frexp:
	fldl 4(%esp)
	movl 12(%esp),%eax
__frexp:
	fxtract
	fxch
	fistpl (%eax)
	pushl $0x3f000000	# 1/2
	fmuls (%esp)
	incl (%eax)
	popl %eax
	ret

frexpl:
	fldt 4(%esp)
	movl 16(%esp),%eax
	jmp __frexp

.global ldexpf,ldexp,ldexpl
	.type ldexpf,@function
	.type ldexp,@function
	.type ldexpl,@function

ldexpf:
	fildl 8(%esp)
	flds 4(%esp)
	fscale
	ret

ldexp:
	fildl 12(%esp)
	fldl 4(%esp)
	fscale
	ret

ldexpl:
	fildl 16(%esp)
	fldt 4(%esp)
	fscale
	ret

.global logbf,logb,logbl
	.type logbf,@function
	.type logb,@function
	.type logbl,@function

#logbf:	flds 4(%esp)
#	fxtract
#	fxch
#	ret

#logb:	fldl 4(%esp)
#	fxtract
#	fxch
#	ret

#logbl:	fldt 4(%esp)
#	fxtract
#	fxch
#	ret

.global ilogbf,ilogb,ilogbl
	.type ilogbf,@function
	.type ilogb,@function
	.type ilogbl,@function

logbf:
ilogbf:	flds 4(%esp)
	jmp __ilogb

logb:
ilogb:	fldl 4(%esp)
__ilogb:
	fxtract
	pushl %eax
	fxch
	fistl (%esp)
	popl %eax
	ret

logbl:
ilogbl:	fldt 4(%esp)
	jmp __ilogb

