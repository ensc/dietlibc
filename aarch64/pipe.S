#include "aarch64-features.h"

FUNC_START	pipe
	mov	x1, #0	/* flags */
	b	pipe2
FUNC_END	pipe
