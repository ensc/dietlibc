
.text
.type   ceil,@function
.global ceil
ceil:
        fldl   4(%esp)
        push   %ecx
        movl   %esp,%ecx
        fnstcw (%ecx)
        movw   0(%ecx),%ax
        andb   $0xFB,%ah
        orb    $0x08,%ah
        movw   %ax,2(%ecx)
        fldcw  2(%ecx)
        frndint
        fldcw  0(%ecx)
        pop    %ecx
        ret

.ende:
.size    ceil,.ende-ceil

