
int __fflush_stdin() __attribute__((weak,alias("__nop")));
int __fflush_stdout() __attribute__((weak,alias("__nop")));
int __fflush_stderr() __attribute__((weak,alias("__nop")));

/* used for weak aliases */
int __nop() { return 0; }
