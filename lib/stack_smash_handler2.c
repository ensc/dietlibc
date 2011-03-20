#include <unistd.h>
#include <write12.h>

void __stack_chk_fail(void);

/* earlier versions of ProPolice actually gave the address and function
 * name as arguments to the handler, so it could print some useful
 * diagnostics.  No more. :-( */
void __stack_chk_fail(void) {
  __write2("smashed stack detected, program terminated.\n");

  /* trigger a segfault which can be inspected within a debugger (inclusive
   * stack-trace). 'abort(3)' at this place would be too heavy weighted.
   *
   * TODO: limit this to systems which are known to have an MMU (resp. is
   * dietlibc with stack-protector used on systems without an MMU?)
   */
  *(char volatile *)0 = 0;
  while (1) {
#if defined(__GNUC__) && ((((__GNUC__) << 8) | (__GNUC_MINOR__)) >= 0x405)
    __builtin_unreachable();
#endif
  }
}
