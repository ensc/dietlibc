#include "syscalls.h"
#include <errno.h>
#include "dietwarning.h"

.text
.type clone,@function
.weak clone
clone:
.type __clone,@function
.global __clone
__clone:
	mov	$-EINVAL, %rax
	ret

link_warning("clone", "warning: clone() not yet implemented")
link_warning("__clone", "warning: clone() not yet implemented")
