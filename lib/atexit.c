typedef void (*function)(void);

#define NUM_ATEXIT	8

static function __atexitlist[NUM_ATEXIT];

int atexit(function t) {
  int i;
  for (i=0; i<NUM_ATEXIT; i++)
    if (__atexitlist[i]==0) {
      __atexitlist[i]=t;
      return 0;
    }
  return -1;
}

extern void _exit(int code) __attribute__((noreturn));

void exit(int code) {
  int i;
  for (i=NUM_ATEXIT;i;) {
    if (__atexitlist[--i])
      __atexitlist[i]();
  }
  _exit(code);
}
