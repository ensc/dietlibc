#include "syscalls.h"
#include "arm-features.h"

#ifdef __ARM_EABI__

@ Calling convention:
@   r0 ... thread data
@ Returns:
@   r0 ... low-level error code
FUNC_START	__arm_set_tls
	mov	ip, r7
	ldr	r7, =__ARM_NR_set_tls
	swi	0
	mov	r7, ip

	@ we do not have much options here to handle errors because
	@ errno might not be available yet. Return error code as-is
	@ and let it handle by the caller

	RET
FUNC_END	__arm_set_tls

#else

#warning "setup-tls only implemented for ARM_EABI!"

#endif
