int __isxdigit_ascii(int c) {
  int d=c|0x20;
  return (((c>='0')&(c<='9')) | ((d>='a')&(d<='f')));
}

int isxdigit(int c) __attribute__((weak,alias("__isxdigit_ascii")));
