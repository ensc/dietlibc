
int __fflush_stdin(void)	 __attribute__((weak,alias("__return0")));
int __fflush_stdout(void)	 __attribute__((weak,alias("__return0")));
int __fflush_stderr(void)	 __attribute__((weak,alias("__return0")));

/* used for weak aliases */
int __return0(void);
int __return0(void) { return 0; }

void endhostent(void)	 __attribute__((weak,alias("__return0")));
void flockfile(void)	 __attribute__((weak,alias("__return0")));
void funlockfile(void)	 __attribute__((weak,alias("__return0")));
