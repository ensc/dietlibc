#define _ASM
#define _SETJMP_H
#include <bits/setjmp.h>

#define ST_FLUSH_WINDOWS        0x03

.section	.rodata
.text
.globl __setjmp
__setjmp:
        b       1f
         set    0, %o1
.size __setjmp,.-__setjmp

.globl setjmp
setjmp:
        set     1, %o1
.size setjmp,.-setjmp

.globl __sigsetjmp
__sigsetjmp:
1:
        /* Save our PC, SP and FP.  Save the signal mask if requested with
           a tail-call for simplicity; it always returns zero.  */
        ta      ST_FLUSH_WINDOWS

        st      %o7, [%o0 + (JB_PC * 4)]
        st      %sp, [%o0 + (JB_SP * 4)]
        st      %fp, [%o0 + (JB_FP * 4)]

        mov     %o7, %g1
        call    __sigjmp_save
         mov    %g1, %o7
.size __sigsetjmp,.-__sigsetjmp

