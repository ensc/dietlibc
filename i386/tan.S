
.text
.type   tan,@function
.global tan
tan:
	fldl    4(%esp)
	fsincos
	fdivr
	ret

.ende:
.size	 tan,.ende-tan
