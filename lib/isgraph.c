int __isgraph_ascii(int c) {
  return (c>=33 && c<=126);
}

int isgraph(int c) __attribute__((weak,alias("__isgraph_ascii")));
