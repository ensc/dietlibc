#include <setjmp.h>
#include "dietwarning.h"

.text
.weak setjmp
.type setjmp,@function
setjmp:
.globl __setjmp
.type __setjmp,@function
__setjmp:
	ret

.globl __sigsetjmp
.type __sigsetjmp,@function
__sigsetjmp:
	ret
.size __sigsetjmp,.-__sigsetjmp;

link_warning("setjmp","error: setjmp() not yet implemented")
link_warning("__sigsetjmp","error: setjmp() not yet implemented")

