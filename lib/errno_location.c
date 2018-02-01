#include <sys/tls.h>
#include <threads.h>
#include <errno.h>
#undef errno
#include <errno_definition.h>

// int *__errno_location(void) __attribute__((weak));
__attribute__((weak)) int *__errno_location(void) { return &errno; }

