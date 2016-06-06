#include "aarch64-features.h"

FUNC_START	__testandset
	mov	x2, x0
	mov	w1, #1
	/* We want __testandset to be a full barrier for operations before and after.
	 * See http://lists.infradead.org/pipermail/linux-arm-kernel/2014-February/229588.html
	 * for details why we want to use ldxr (instead of ldaxr) here and a full memory
	 * barrier afterwards. */
1:	ldxr	w0, [x2]
	stlxr	w2, w1, [x2]
	cbnz	w2, 1b
	dmb	ish
	ret
FUNC_END	__testandset
