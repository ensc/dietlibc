#include <unistd.h>

__attribute__((weak)) int pthread_exit() { exit(0); }
