
int __isprint_ascii(int c) {
  return (c>=32 && c<=126);
}

int isprint(int c) __attribute__((weak,alias("__isprint_ascii")));
