typedef void (*function)(void);

static function __atexitlist[4];

int atexit(function t) {
  int i;
  for (i=0; i<4; i++)
    if (__atexitlist[i]==0) {
      __atexitlist[i]=t;
      return 0;
    }
  return -1;
}

extern void _exit(int code) __attribute__((noreturn));

void exit(int code) {
  if (__atexitlist[3]) __atexitlist[3]();
  if (__atexitlist[2]) __atexitlist[2]();
  if (__atexitlist[1]) __atexitlist[1]();
  if (__atexitlist[0]) __atexitlist[0]();
  _exit(code);
}
