/* diet includes this file to create linker dependencies on the diet
 * libc, so trying to link an object file compiled with diet against
 * glibc will fail. */

#ifndef NODIETREF
#include <endian.h>
#ifdef __ASSEMBLER__
.section .note
#if (__WORDSIZE == 64)
.quad __you_tried_to_link_a_dietlibc_object_against_glibc
#else
.long __you_tried_to_link_a_dietlibc_object_against_glibc
#endif
.previous
#else
#if (__WORDSIZE == 64)
__asm__ (".section .note\n\t.quad __you_tried_to_link_a_dietlibc_object_against_glibc\n\t.previous");
#else
__asm__ (".section .note\n\t.long __you_tried_to_link_a_dietlibc_object_against_glibc\n\t.previous");
#endif
#endif
#endif
