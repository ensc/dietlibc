
int __fflush_stdin()	 __attribute__((weak,alias("__return0")));
int __fflush_stdout()	 __attribute__((weak,alias("__return0")));
int __fflush_stderr()	 __attribute__((weak,alias("__return0")));

/* used for weak aliases */
int __return0() { return 0; }
