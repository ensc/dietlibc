.text
.type   fmod,@function
.global fmod
fmod:
	fldl  12(%esp)
	fldl   4(%esp)
	fprem
	fstp    %st(1)
	ret

.ende:
.size    fmod,.ende-fmod
