#ifndef _STDARG_H
#define _STDARG_H

#include <endian.h>

typedef char* va_list;

#define va_end(ap) ap=0

#ifdef __sparc__

enum __va_type_classes {
  __no_type_class = -1,
  __void_type_class,
  __integer_type_class,
  __char_type_class,
  __enumeral_type_class,
  __boolean_type_class,
  __pointer_type_class,
  __reference_type_class,
  __offset_type_class,
  __real_type_class,
  __complex_type_class,
  __function_type_class,
  __method_type_class,
  __record_type_class,
  __union_type_class,
  __array_type_class,
  __string_type_class,
  __set_type_class,
  __file_type_class,
  __lang_type_class
};

#define va_start(AP, LASTARG) \
  (__builtin_next_arg (LASTARG), AP = (char *) __builtin_saveregs ())

#define __va_rounded_size(TYPE)  \
  (((sizeof (TYPE) + sizeof (int) - 1) / sizeof (int)) * sizeof (int))

/* We don't declare the union member `d' to have type TYPE
   because that would lose in C++ if TYPE has a constructor.  */
/* We cast to void * and then to TYPE * because this avoids
   a warning about increasing the alignment requirement.
   The casts to char * avoid warnings about invalid pointer arithmetic.  */
#define va_arg(pvar,TYPE)					\
__extension__							\
(*({((__builtin_classify_type (*(TYPE*) 0) >= __record_type_class \
      || (__builtin_classify_type (*(TYPE*) 0) == __real_type_class \
	  && sizeof (TYPE) == 16))				\
    ? ((pvar) = (char *)(pvar) + __va_rounded_size (TYPE *),	\
       *(TYPE **) (void *) ((char *)(pvar) - __va_rounded_size (TYPE *))) \
    : __va_rounded_size (TYPE) == 8				\
    ? ({ union {char __d[sizeof (TYPE)]; int __i[2];} __u;	\
	 __u.__i[0] = ((int *) (void *) (pvar))[0];		\
	 __u.__i[1] = ((int *) (void *) (pvar))[1];		\
	 (pvar) = (char *)(pvar) + 8;				\
	 (TYPE *) (void *) __u.__d; })				\
    : ((pvar) = (char *)(pvar) + __va_rounded_size (TYPE),	\
       ((TYPE *) (void *) ((char *)(pvar) - __va_rounded_size (TYPE)))));}))


#else	/* !__sparc__ */

/* this only works when everything is passed on the stack (i.e. x86) */
#if __WORDSIZE == 64
#define va_start(ap,argn) ap=((char*)&argn)+8
#else
#define va_start(ap,argn) ap=((char*)&argn)+4
#endif
#define va_arg(ap,type) (ap+=sizeof(type), *(type*)((void*)ap-sizeof(type)))

#endif

#endif
