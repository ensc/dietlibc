#define a2      $6
#define a3      $7
#define sp      $29     /* stack pointer */
#define fp      $30     /* frame pointer */
#define t9      $25

.text
.global __sigsetjmp
.type __sigsetjmp,@function
__sigsetjmp:
	la	t9, __sigsetjmp_aux
	move	a2, sp
	move	a3, fp
	jr	t9

