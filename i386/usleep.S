.text
.global usleep
.type    usleep,@function

usleep:
        movl    4(%esp),%eax
        xorl    %edx,%edx
        movl    $1000000,%ecx
        divl    %ecx
        imull   $1000,%edx
        pushl   %edx
        pushl   %eax
        movl    %esp,%eax
        pushl   %eax
        pushl   %eax
        call    nanosleep
        addl    $16,%esp
        ret

.Lende:
.size    usleep,.Lende-usleep

