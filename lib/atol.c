long int atol(const char* s) {
  long int v=0;
  int sign=1;
  while (isspace(*s)) ++s;
  if (*s=='-') { sign=-1; ++s; } else if (*s=='+') ++s;
  while (*s && (*s>='0') && (*s<='9'))
    v=v*10+*s++-'0';
  return v*sign;
}
