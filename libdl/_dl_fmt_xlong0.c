static inline char tohex(char c) {
  return c>=10?c-10+'a':c+'0';
}

static void _dl_fmt_xlong0(char*dest,unsigned long n) {
  int i=8;
  while (i) {
    dest[--i]=tohex(n&15);
    n>>=4;
  }
}
