
.text
.type   floor,@function
.global floor
floor:
        fldl   4(%esp)
        push   %ecx
        movl   %esp,%ecx
        fnstcw (%ecx)
        movw   0(%ecx),%ax
        andb   $0xF7,%ah
        orb    $0x04,%ah
        movw   %ax,2(%ecx)
        fldcw  2(%ecx)
        frndint
        fldcw  0(%ecx)
        pop    %ecx
        ret

.ende:
.size    floor,.ende-floor

