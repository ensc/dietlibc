
.text
.type   rint,@function
.global rint

rint:
        fldl   4(%esp)
        push   %ecx
        movl   %esp,%ecx
        fnstcw (%ecx)
        movw   0(%ecx),%ax
        andb   $0xF7,%ah
        movw   %ax,2(%ecx)
        fldcw  2(%ecx)
        frndint
        fldcw  0(%ecx)
        pop    %ecx
        ret

.Lende:
.size    rint,.Lende-rint
