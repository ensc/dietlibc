
.text
.type   copysign,@function
.global copysign

# double  copysign ( double value, double sign );

copysign:
        movb  12+7(%esp),%al
        movb   4+7(%esp),%ah
        andw  $0x7F80,%ax
        orb   %ah,%al
        movb   4+7(%esp),%al
        fldl   4(%esp)
        ret

.ende:
.size    copysign,.ende-copysign
