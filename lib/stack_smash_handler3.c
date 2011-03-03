#include <sys/cdefs.h>

extern void __stack_chk_fail(void) __attribute__((noreturn));

/* no idea why sometimes this is called instead of __stack_chk_fail,
 * but it apparently only happens with shared libraries */

void __attribute__((noreturn)) __stack_chk_fail_local(void)
{
  __stack_chk_fail ();
}
