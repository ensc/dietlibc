.data
.align 4
.type   seed,@object
.size   seed,4
seed:   .long 1

.text
.global rand
.type   rand,@function
.global random
.type   random,@function

rand:
random:
        imull   $1103515245,seed,%eax
        addl    $12345,%eax
store:  movl    %eax,seed
        andl    $0x7FFFFFFF,%eax
        ret

.global srand
.type   srand,@function
.global srandom
.type   srandom,@function

srand:
srandom:
        movl    4(%esp),%eax
        jmp     store
