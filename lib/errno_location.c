extern int errno;

int *__errno_location() { return &errno; }

int *errno_location() __attribute__((weak,alias("__errno_location")));
