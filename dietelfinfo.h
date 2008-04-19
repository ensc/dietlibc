#include "dietfeatures.h"

#ifdef WANT_ELFINFO
#include <elf.h>
#include <endian.h>
#include <stdint.h>

/* TODO: exported interface from <linux/elf.h> has been changed in 2.6.25 so
 * the 'elf_addr_t' type is not available anymore. Hence, derive it from
 * __WORDSIZE__. */

#if __WORDSIZE == 64
typedef uint64_t	__diet_elf_addr_t;
#elif __WORDSIZE == 32
typedef uint32_t	__diet_elf_addr_t;
#endif

__diet_elf_addr_t const *	__get_elf_aux_value(unsigned int tag)
	__attribute__((__visibility__("hidden"),__const__)) __pure;
#endif
