/* diet includes this file to create linker dependencies on the diet
 * libc, so trying to link an object file compiled with diet against
 * glibc will fail. */

asm (".section .note\n\t.long __you_tried_to_link_a_dietlibc_object_against_glibc\n\t.previous");
