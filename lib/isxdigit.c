int __isxdigit_ascii(int c) {
  return ((c>='0'&&c<='9') || (c>='A'&&c<='F') || (c>='a'&&c<='f'));
}

int isxdigit(int c) __attribute__((weak,alias("__isxdigit_ascii")));
