#ifndef _DLFCN_H
#define _DLFCN_H 1

#define RTLD_LAZY	0x00000
#define RTLD_NOW	0x00001

#define RTLD_LOCAL	0x00000
#define RTLD_GLOBAL	0x10000

void *dlopen (const char *filename, int flag);
const char *dlerror(void);
void *dlsym(void *handle, char *symbol);
int dlclose (void *handle);

#endif
