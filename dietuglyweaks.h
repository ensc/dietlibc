#ifndef __DIET_UGLY_WEAKS__
#define __DIET_UGLY_WEAKS__

/* if you change something here ... KNOW what you're doing !
 * it'll effect ALL platforms ! */

#ifdef __clang__
.macro DEF_G name
.global \name
#ifdef __PIE__
.hidden \name
#endif
.type \name,@function
\name:
.endm
.macro DEF_W name
.weak \name
#ifdef __PIE__
.hidden \name
#endif
.type \name,@function
\name:
.endm
#else
.macro DEF_G name
.global \name
#ifdef __PIE__
.hidden \name
#endif
.type \name,function
\name:
.endm
.macro DEF_W name
.weak \name
#ifdef __PIE__
.hidden \name
#endif
.type \name,function
\name:
.endm
#endif

DEF_W __fflush_stderr
DEF_W __fflush_stdin
DEF_W __fflush_stdout
DEF_W __nop
DEF_W __thread_doexit
DEF_W flockfile
DEF_W ftrylockfile
DEF_W funlockfile
DEF_W __register_frame_info
DEF_W __deregister_frame_info

DEF_G __you_tried_to_link_a_dietlibc_object_against_glibc

#endif
