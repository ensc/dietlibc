#ifndef H_DIETLIBC_UNALIGNED_H
#define H_DIETLIBC_UNALIGNED_H

#include <string.h>
#include <inttypes.h>

#define read_unaligned(_src)						\
	({								\
		struct {						\
			__typeof__(*(_src))	a;			\
		} __attribute__((__packed__)) *_tmp = (void *)(_src);	\
		_tmp->a;						\
	})

#define read_unaligned_s(_src, _type, _attr)				\
	read_unaligned((__typeof__(&((_type *)(0))->_attr))(		\
			       (uintptr_t)(_src) +			\
			       __builtin_offsetof(_type, _attr)))	\


#define write_unaligned(_v, _dst)		\
	({								\
		struct {						\
			__typeof__(*(_dst))	a;			\
		} __attribute__((__packed__)) *_tmp = (void *)(_dst);	\
		_tmp->a = (_v);						\
	})

#define write_unaligned_s(_v, _dst, _type, _attr)			\
	write_unaligned(_v,						\
			(__typeof__(&((_type *)(0))->_attr))(		\
				(uintptr_t)(_dst) +			\
				__builtin_offsetof(_type, _attr)))	\

#endif	/* H_DIETLIBC_UNALIGNED_H */
