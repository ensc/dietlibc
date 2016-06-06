/* --*- asm -*-- */

#ifndef H_DIETLIBC_PPC64_FEATURES_H
#define H_DIETLIBC_PPC64_FEATURES_H

#if defined(_CALL_ELF) && _CALL_ELF == 2
/* ELFv2 ABI for ppc64el (LE) */

#define FN_NAME_DOTIFY(name) name


#else
/* ELFv1 ABI for ppc64 (BE)*/

#define FN_NAME_DOTIFY(name) .##name

#endif /* _CALL_ELF == 2 */


#endif	/* H_DIETLIBC_PPC64_FEATURES_H */
