#include <sys/tls.h>
#include <threads.h>
#include <errno.h>

// int *__errno_location(void) __attribute__((weak));
__attribute__((weak)) int *__errno_location(void) { return &errno; }

