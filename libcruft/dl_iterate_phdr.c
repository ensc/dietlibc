/* support function when linking against libgcc_eh.a
 *
 * see gcc sources _Unwind_Find_FDE() in unwind-dw2-fde-glibc.c
 *
 * Copyright (C) 2005 Markus F.X.J. Oberhumer
 * License: GNU GPL
 */

#include <limits.h>
#include <stddef.h>
#include <elf.h>

#if __WORDSIZE == 64
#  define ElfW(type) Elf64_##type
#else
#  define ElfW(type) Elf32_##type
#endif

struct dl_phdr_info
{
    ElfW(Addr) dlpi_addr;
    const char *dlpi_name;
    const ElfW(Phdr) *dlpi_phdr;
    ElfW(Half) dlpi_phnum;
};

int dl_iterate_phdr(int (*callback) (struct dl_phdr_info *info, size_t size, void *data),
                    void *data);
int dl_iterate_phdr(int (*callback) (struct dl_phdr_info *info, size_t size, void *data),
                    void *data)
{
    if (!callback)
        return -1;
    (void)data;
    return -1;
}


