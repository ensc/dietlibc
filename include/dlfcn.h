#ifndef _DLFCN_H
#define _DLFCN_H 1

#include <sys/cdefs.h>

#define RTLD_LAZY	0x00000
#define RTLD_NOW	0x00001

#define RTLD_LOCAL	0x00000
#define RTLD_GLOBAL	0x10000

#define RTLD_DEFAULT	((void*)1)
#define RTLD_NEXT	((void*)2)

__BEGIN_DECLS

void *dlopen (const char *filename, int flag);
const char *dlerror(void);
void *dlsym(void *handle, char *symbol);
int dlclose (void *handle);

__END_DECLS

#endif
