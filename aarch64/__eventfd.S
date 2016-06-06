#include "aarch64-features.h"

FUNC_START	__eventfd
	mov	x1, #0
	b	__eventfd2
FUNC_END	__eventfd
