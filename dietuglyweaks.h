#ifndef __DIET_UGLY_WEAKS__
#define __DIET_UGLY_WEAKS__

/* if you change something here ... KNOW what you're doing !
 * it'll effect ALL platforms ! */

.weak __fflush_stdin
__fflush_stdin:
.weak __fflush_stdout
__fflush_stdout:
.weak __fflush_stderr
__fflush_stderr:
.weak flockfile
flockfile:
.weak funlockfile
funlockfile:
.global __you_tried_to_link_a_dietlibc_object_against_glibc
__you_tried_to_link_a_dietlibc_object_against_glibc:

#endif
