#include <setjmp.h>
#include "dietwarning.h"

void __longjmp(jmp_buf env, int val);
void __longjmp(jmp_buf env, int val)
{
	return;
}

link_warning("__longjmp","error: __longjmp not yet supported")

