#ifndef _STDARG_H
#define _STDARG_H

#include <endian.h>

#if defined(__sparc__)

typedef char* va_list;
#define va_end(ap) ap=0

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


#elif defined(__mips__)

typedef __builtin_va_list va_list;
#define va_start(v,l)	__builtin_stdarg_start((v),l)
#define va_end		__builtin_va_end
#define va_arg		__builtin_va_arg
#if !defined(__STRICT_ANSI__) || __STDC_VERSION__ + 0 >= 199900L
#define va_copy(d,s)	__builtin_va_copy((d),(s))
#endif
#define __va_copy(d,s)	__builtin_va_copy((d),(s))

#elif defined(__powerpc__)

typedef struct __va_list_tag {
  unsigned char gpr;		/* index into the array of 8 GPRs stored in the
				   register save area gpr=0 corresponds to r3,
				   gpr=1 to r4, etc. */
  unsigned char fpr;		/* index into the array of 8 FPRs stored in the
				   register save area fpr=0 corresponds to f1,
				   fpr=1 to f2, etc. */
  char *overflow_arg_area;	/* location on stack that holds the next
				   overflow argument */
  char *reg_save_area;		/* where r3:r10 and f1:f8, if saved are stored */
} va_list[1];

#define __va_overflow(AP) (AP)->overflow_arg_area
#ifdef __OPTIMIZE__
extern void __va_arg_type_violation(void) __attribute__((__noreturn__));
#else
#define __va_arg_type_violation()
#endif

typedef struct {
  long   __gp_save[8];		/* save area for GP registers */
  double __fp_save[8];		/* save area for FP registers */
} __va_regsave_t;

/* Macros to access the register save area */
/* We cast to void * and then to TYPE * because this avoids
   a warning about increasing the alignment requirement.  */
#define __VA_FP_REGSAVE(AP,OFS,TYPE)					\
  ((TYPE *) (void *) (&(((__va_regsave_t *)				\
			 (AP)->reg_save_area)->__fp_save[OFS])))

#define __VA_GP_REGSAVE(AP,OFS,TYPE)					\
  ((TYPE *) (void *) (&(((__va_regsave_t *)				\
			 (AP)->reg_save_area)->__gp_save[OFS])))

#define __va_start_common(AP, FAKE) \
  __builtin_memcpy ((AP), __builtin_saveregs (), sizeof(va_list))

#define va_start(AP,LASTARG) \
  (__builtin_next_arg (LASTARG), __va_start_common (AP, 0))

#ifdef _SOFT_FLOAT
#define __va_float_p(TYPE)	0
#else
#define __va_float_p(TYPE)	(__builtin_classify_type(*(TYPE *)0) == 8)
#endif

#define __va_aggregate_p(TYPE)	(__builtin_classify_type(*(TYPE *)0) >= 12)
#define __va_size(TYPE)		((sizeof(TYPE) + sizeof (long) - 1) / sizeof (long))

#define va_arg(AP,TYPE)							   \
__extension__ (*({							   \
  register TYPE *__ptr;							   \
									   \
  if (__va_float_p (TYPE) && sizeof (TYPE) < 16)			   \
    {									   \
      unsigned char __fpr = (AP)->fpr;					   \
      if (__fpr < 8)							   \
	{								   \
	  __ptr = __VA_FP_REGSAVE (AP, __fpr, TYPE);			   \
	  (AP)->fpr = __fpr + 1;					   \
	}								   \
      else if (sizeof (TYPE) == 8)					   \
	{								   \
	  unsigned long __addr = (unsigned long) (__va_overflow (AP));	   \
	  __ptr = (TYPE *)((__addr + 7) & -8);				   \
	  __va_overflow (AP) = (char *)(__ptr + 1);			   \
	}								   \
      else								   \
	{								   \
	  /* float is promoted to double.  */				   \
	  __va_arg_type_violation ();					   \
	}								   \
    }									   \
									   \
  /* Aggregates and long doubles are passed by reference.  */		   \
  else if (__va_aggregate_p (TYPE) || __va_float_p (TYPE))		   \
    {									   \
      unsigned char __gpr = (AP)->gpr;					   \
      if (__gpr < 8)							   \
	{								   \
	  __ptr = * __VA_GP_REGSAVE (AP, __gpr, TYPE *);		   \
	  (AP)->gpr = __gpr + 1;					   \
	}								   \
      else								   \
	{								   \
	  TYPE **__pptr = (TYPE **) (__va_overflow (AP));		   \
	  __ptr = * __pptr;						   \
	  __va_overflow (AP) = (char *) (__pptr + 1);			   \
	}								   \
    }									   \
									   \
  /* Only integrals remaining.  */					   \
  else									   \
    {									   \
      /* longlong is aligned.  */					   \
      if (sizeof (TYPE) == 8)						   \
	{								   \
	  unsigned char __gpr = (AP)->gpr;				   \
	  if (__gpr < 7)						   \
	    {								   \
	      __gpr += __gpr & 1;					   \
	      __ptr = __VA_GP_REGSAVE (AP, __gpr, TYPE);		   \
	      (AP)->gpr = __gpr + 2;					   \
	    }								   \
	  else								   \
	    {								   \
	      unsigned long __addr = (unsigned long) (__va_overflow (AP)); \
	      __ptr = (TYPE *)((__addr + 7) & -8);			   \
	      (AP)->gpr = 8;						   \
	      __va_overflow (AP) = (char *)(__ptr + 1);			   \
	    }								   \
	}								   \
      else if (sizeof (TYPE) == 4)					   \
	{								   \
	  unsigned char __gpr = (AP)->gpr;				   \
	  if (__gpr < 8)						   \
	    {								   \
	      __ptr = __VA_GP_REGSAVE (AP, __gpr, TYPE);		   \
	      (AP)->gpr = __gpr + 1;					   \
	    }								   \
	  else								   \
	    {								   \
	      __ptr = (TYPE *) __va_overflow (AP);			   \
	      __va_overflow (AP) = (char *)(__ptr + 1);			   \
	    }								   \
	}								   \
      else								   \
	{								   \
	  /* Everything else was promoted to int.  */			   \
	  __va_arg_type_violation ();					   \
	}								   \
    }									   \
  __ptr;								   \
}))

#define va_end(AP)	((void)0)

/* Copy va_list into another variable of this type.  */
#define va_copy(dest, src) *(dest) = *(src)

#else	/* !__sparc__ && !__powerpc__ */

typedef char* va_list;

#ifndef __i386__
#warning "stdarg for this platform is untested!"
#endif

/* this only works when everything is passed on the stack (i.e. x86) */
#if __WORDSIZE == 64
#define va_start(ap,argn) ap=((char*)&argn)+8
#else
#define va_start(ap,argn) ap=((char*)&argn)+4
#endif
#define va_arg(ap,type) (ap+=sizeof(type), *(type*)((void*)ap-sizeof(type)))

#endif

#ifndef va_copy
#define va_copy(x,y) x=y
#endif
#ifndef va_end
#define va_end(ap) ap=0
#endif

#endif
