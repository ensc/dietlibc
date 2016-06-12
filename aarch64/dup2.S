#include "aarch64-features.h"

FUNC_START	dup2
	mov	x2, #0
	b	dup3
FUNC_END	dup2
