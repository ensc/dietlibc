.text

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

.Lende:
.size    floor,.Lende-floor
.size    floorl,.Lende-floorl
.size    floorf,.Lende-floorf
